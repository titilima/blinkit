// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: classic_pending_script.cpp
// Description: ClassicPendingScript Class
//      Author: Ziming Li
//     Created: 2020-02-02
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "classic_pending_script.h"

#include "base/memory/ptr_util.h"
#include "blinkit/crawler/crawler_impl.h"
#include "third_party/blink/renderer/bindings/core/duk/script_source_code.h"
#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/platform/loader/fetch/fetch_parameters.h"

using namespace BlinKit;

namespace blink {

ClassicPendingScript::ClassicPendingScript(
    ScriptElementBase *element,
    const TextPosition &startingPosition,
    ScriptSourceLocationType sourceLocationType,
    bool isExternal)
    : PendingScript(element, startingPosition)
    , m_baseUrlForInlineScript(isExternal ? BkURL() : element->GetDocument().BaseURL())
    , m_sourceTextForInlineScript(isExternal ? String() : element->TextFromChildren())
    , m_sourceLocationType(sourceLocationType)
    , m_isExternal(isExternal)
    , m_readyState(isExternal ? kWaitingForResource : kReady)
    , m_notStreamedReason(isExternal ? ScriptStreamer::kDidntTryToStartStreaming : ScriptStreamer::kInlineScript)
{
}

ClassicPendingScript::~ClassicPendingScript(void) = default;

void ClassicPendingScript::AdvanceReadyState(ReadyState newReadyState)
{
    // We will allow exactly these state transitions:
    //
    // kWaitingForResource -> kWaitingForStreaming -> [kReady, kErrorOccurred]
    // kReady -> kReadyStreaming -> kReady
    switch (m_readyState)
    {
        case kWaitingForResource:
            ASSERT(kWaitingForStreaming == newReadyState);
            break;
        case kWaitingForStreaming:
            ASSERT(kReady == newReadyState || kErrorOccurred == newReadyState);
            break;
        case kReady:
            ASSERT(kReadyStreaming == newReadyState);
            break;
        case kReadyStreaming:
            ASSERT(kReady == newReadyState);
            break;
        case kErrorOccurred:
            NOTREACHED();
            break;
    }

    bool oldIsReady = IsReady();
    m_readyState = newReadyState;

    // Did we transition into a 'ready' state?
    if (IsReady() && !oldIsReady && IsWatchingForLoad())
        PendingScriptFinished();

    // Did we finish streaming?
    if (IsCurrentlyStreaming())
    {
        if (kReady == m_readyState || kErrorOccurred == m_readyState)
        {
            // Call the streamer_done_ callback. Ensure that is_currently_streaming_
            // is reset only after the callback returns, to prevent accidentally
            // start streaming by work done within the callback. (crbug.com/754360)
            std::function<void()> done;
            done.swap(m_streamerDone);
            if (done)
                done();
            m_isCurrentlyStreaming = false;
        }
    }

    // Streaming-related post conditions:

    // To help diagnose crbug.com/78426, we'll temporarily add some DCHECKs
    // that are a subset of the DCHECKs below:
    if (IsCurrentlyStreaming())
    {
        ASSERT(m_streamer);
        ASSERT(!m_streamer->IsFinished());
    }

    // IsCurrentlyStreaming should match what streamer_ thinks.
    ASSERT(IsCurrentlyStreaming() == (m_streamer && !m_streamer->IsFinished()));
    // IsCurrentlyStreaming should match the ready_state_.
    ASSERT(IsCurrentlyStreaming() ==
        (kReadyStreaming == m_readyState
            || (m_streamer 
                && (kWaitingForResource == m_readyState || kWaitingForStreaming == m_readyState))));
    // We can only have a streamer_done_ callback if we are actually streaming.
    ASSERT(IsCurrentlyStreaming() || !m_streamerDone);
}

void ClassicPendingScript::CancelStreaming(void)
{
    if (!m_streamer)
        return;

    m_streamer->Cancel();
    m_streamer.reset();
    m_streamerDone = std::function<void()>();
    m_isCurrentlyStreaming = false;
    ASSERT(!IsCurrentlyStreaming());
}

void ClassicPendingScript::CheckState(void) const
{
    // TODO(hiroshige): Turn these CHECK()s into DCHECK() before going to beta.
    ASSERT(nullptr != GetElement());
    ASSERT(m_isExternal == (nullptr != GetResource()));
    ASSERT(nullptr != GetResource() || !m_streamer);
}

std::shared_ptr<ClassicPendingScript> ClassicPendingScript::CreateInline(
    ScriptElementBase *element,
    const TextPosition &startingPosition,
    ScriptSourceLocationType sourceLocationType)
{
    std::shared_ptr<ClassicPendingScript> pendingScript = base::WrapShared(new ClassicPendingScript(element, startingPosition, sourceLocationType, false));
    pendingScript->CheckState();
    return pendingScript;
}

void ClassicPendingScript::DataReceived(Resource *resource, const char *data, size_t size)
{
    if (m_streamer)
        m_streamer->NotifyAppendData(ToScriptResource(resource));
}

void ClassicPendingScript::DisposeInternal(void)
{
    ClearResource();
    m_integrityFailure = false;
    CancelStreaming();
}

std::shared_ptr<ClassicPendingScript> ClassicPendingScript::Fetch(
    const BkURL &url,
    Document &elementDocument,
    const WTF::TextEncoding &encoding,
    ScriptElementBase *element)
{
    ResourceRequest request(url);
    if (elementDocument.ForCrawler())
    {
        CrawlerImpl *crawler = ToCrawlerImpl(elementDocument.GetFrame()->Client());
        request.SetCrawler(crawler);
        request.SetHijackType(HijackType::kScript);
    }

    ResourceLoaderOptions resourceLoaderOptions;
    resourceLoaderOptions.initiator_info.name = "script";

    FetchParameters params(request, resourceLoaderOptions);

    std::shared_ptr<ClassicPendingScript> pendingScript = base::WrapShared(new ClassicPendingScript(
        element, TextPosition(), ScriptSourceLocationType::kExternalFile,
        true));

#if 0 // BKTODO: Check if necessary.
    // [Intervention]
    // For users on slow connections, we want to avoid blocking the parser in
    // the main frame on script loads inserted via document.write, since it
    // can add significant delays before page content is displayed on the
    // screen.
    pendingScript->intervened_ =
        MaybeDisallowFetchForDocWrittenScript(params, element_document);
#endif

    // <spec
    // href="https://html.spec.whatwg.org/multipage/webappapis.html#fetch-a-classic-script"
    // step="2">Set request's client to settings object.</spec>
    //
    // Note: |element_document| corresponds to the settings object.
    ScriptResource::Fetch(params, elementDocument.Fetcher(), pendingScript.get());
    pendingScript->CheckState();
    return pendingScript;
}

void ClassicPendingScript::FinishReadyStreaming(void)
{
    CheckState();
    ASSERT(nullptr != GetResource());
    ASSERT(kReadyStreaming == m_readyState);
    AdvanceReadyState(kReady);
}

void ClassicPendingScript::FinishWaitingForStreaming(void)
{
    CheckState();
    ASSERT(nullptr != GetResource());
    ASSERT(kWaitingForStreaming == m_readyState);

    bool errorOccurred = GetResource()->ErrorOccurred();
    AdvanceReadyState(errorOccurred ? kErrorOccurred : kReady);
}

std::unique_ptr<Script> ClassicPendingScript::GetSource(const BkURL &documentURL) const
{
    CheckState();
    ASSERT(IsReady());

    if (m_readyState == kErrorOccurred)
        return nullptr;

    if (!m_isExternal)
    {
        ASSERT(!m_streamer);
        ASSERT(m_notStreamedReason == ScriptStreamer::kInlineScript);

        ScriptSourceCode sourceCode(m_sourceTextForInlineScript, m_sourceLocationType, documentURL, StartingPosition());
        return ClassicScript::Create(sourceCode, m_baseUrlForInlineScript);
    }

    ASSERT(GetResource()->IsLoaded());
    ScriptResource *resource = ToScriptResource(GetResource());

    // Check if we can use the script streamer.
    bool streamerReady = false;
    ScriptStreamer::NotStreamingReason notStreamedReason = m_notStreamedReason;
    if (m_streamer)
    {
        ASSERT(ScriptStreamer::kInvalid == notStreamedReason);
        if (m_streamer->StreamingSuppressed())
        {
            notStreamedReason = m_streamer->StreamingSuppressedReason();
        }
        else if (kErrorOccurred == m_readyState)
        {
            notStreamedReason = ScriptStreamer::kErrorOccurred;
        }
        else if (kReadyStreaming == m_readyState)
        {
            notStreamedReason = ScriptStreamer::kStreamerNotReadyOnGetSource;
        }
        else
        {
            // Streamer can be used to compile script.
            ASSERT(kReady == m_readyState);
            streamerReady = true;
        }
    }

    ScriptSourceCode sourceCode(streamerReady ? m_streamer.get() : nullptr, resource, notStreamedReason);
    // The base URL for external classic script is
    // "the URL from which the script was obtained" [spec text]
    // https://html.spec.whatwg.org/multipage/webappapis.html#concept-script-base-url
    const BkURL &baseURL = sourceCode.Url();
    return ClassicScript::Create(sourceCode, baseURL);
}

bool ClassicPendingScript::IsReady(void) const
{
    CheckState();
    return m_readyState >= kReady;
}

void ClassicPendingScript::NotifyFinished(Resource *resource)
{
    // The following SRI checks need to be here because, unfortunately, fetches
    // are not done purely according to the Fetch spec. In particular,
    // different requests for the same resource do not have different
    // responses; the memory cache can (and will) return the exact same
    // Resource object.
    //
    // For different requests, the same Resource object will be returned and
    // will not be associated with the particular request.  Therefore, when the
    // body of the response comes in, there's no way to validate the integrity
    // of the Resource object against a particular request (since there may be
    // several pending requests all tied to the identical object, and the
    // actual requests are not stored).
    //
    // In order to simulate the correct behavior, Blink explicitly does the SRI
    // checks here, when a PendingScript tied to a particular request is
    // finished (and in the case of a StyleSheet, at the point of execution),
    // while having proper Fetch checks in the fetch module for use in the
    // fetch JavaScript API. In a future world where the ResourceFetcher uses
    // the Fetch algorithm, this should be fixed by having separate Response
    // objects (perhaps attached to identical Resource objects) per request.
    //
    // See https://crbug.com/500701 for more information.
    CheckState();

    // We are now waiting for script streaming to finish.
    // If there is no script streamer, this step completes immediately.
    AdvanceReadyState(kWaitingForStreaming);
    if (m_streamer)
        m_streamer->NotifyFinished();
    else
        FinishWaitingForStreaming();
}

void ClassicPendingScript::SetStreamer(std::unique_ptr<ScriptStreamer> &streamer)
{
    ASSERT(streamer);
    ASSERT(!m_streamer);
    ASSERT(!IsWatchingForLoad() || kWaitingForResource != m_readyState);
    ASSERT(!streamer->IsFinished());
    ASSERT(kWaitingForResource == m_readyState || kReady == m_readyState);

    m_streamer = std::move(streamer);
    m_isCurrentlyStreaming = true;
    if (streamer && kReady == m_readyState)
        AdvanceReadyState(kReadyStreaming);

    CheckState();
}

bool ClassicPendingScript::StartStreamingIfPossible(const std::function<void()> &done)
{
    if (IsCurrentlyStreaming())
        return false;

    // We can start streaming in two states: While still loading
    // (kWaitingForResource), or after having loaded (kReady).
    if (kWaitingForResource != m_readyState  && kReady != m_readyState)
        return false;

    Document *document = &(GetElement()->GetDocument());
    if (nullptr == document || nullptr == document->GetFrame())
        return false;

    // To support streaming re-try, we'll clear the existing streamer if
    // it exists; it claims to be finished; but it's finished because streaming
    // has been suppressed.
    if (m_streamer && m_streamer->StreamingSuppressed() && m_streamer->IsFinished())
    {
        ASSERT(kReady == m_readyState);
        ASSERT(!m_streamerDone);
        ASSERT(!IsCurrentlyStreaming());
        m_streamer.reset();
    }

    if (m_streamer)
        return false;

    // The two checks above should imply that we're not presently streaming.
    ASSERT(!IsCurrentlyStreaming());

    // Parser blocking scripts tend to do a lot of work in the 'finished'
    // callbacks, while async + in-order scripts all do control-like activities
    // (like posting new tasks). Use the 'control' queue only for control tasks.
    // (More details in discussion for cl 500147.)
    auto taskType = GetSchedulingType() == ScriptSchedulingType::kParserBlocking
        ? TaskType::kNetworking
        : TaskType::kNetworkingControl;

    ASSERT(!m_streamer);
    ASSERT(!IsCurrentlyStreaming());
    ASSERT(!m_streamerDone);
    std::shared_ptr<base::SingleThreadTaskRunner> taskRunner = document->GetTaskRunner(taskType);
    ScriptStreamer::StartStreaming(this, taskRunner, &m_notStreamedReason);
    ASSERT(m_streamer || ScriptStreamer::kInvalid != m_notStreamedReason);
    bool success = m_streamer && !m_streamer->IsStreamingFinished();

    // If we have successfully started streaming, we are required to call the
    // callback.
    ASSERT(IsCurrentlyStreaming() == success);
    if (success)
        m_streamerDone = done;
    return success;
}

void ClassicPendingScript::StreamingFinished(void)
{
    std::shared_ptr<ClassicPendingScript> guard(shared_from_this());

    CheckState();
    // BKTODO: DCHECK(streamer_);  // Should only be called by ScriptStreamer.
    ASSERT(IsCurrentlyStreaming());

    if (kWaitingForStreaming == m_readyState)
        FinishWaitingForStreaming();
    else if (kReadyStreaming == m_readyState)
        FinishReadyStreaming();
    else
        NOTREACHED();

    ASSERT(!IsCurrentlyStreaming());
}

bool ClassicPendingScript::WasCanceled(void) const
{
    if (!m_isExternal)
        return false;
    return GetResource()->WasCanceled();
}

} // namespace blink
