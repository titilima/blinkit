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
#include "third_party/blink/renderer/bindings/core/duk/script_source_code.h"
#include "third_party/blink/renderer/core/dom/document.h"

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

void ClassicPendingScript::CancelStreaming(void)
{
    if (!m_streamer)
        return;

    ASSERT(false); // BKTODO:
#if 0
    streamer_->Cancel();
    streamer_ = nullptr;
    streamer_done_.Reset();
    is_currently_streaming_ = false;
    DCHECK(!IsCurrentlyStreaming());
#endif
}

void ClassicPendingScript::CheckState(void) const
{
    // TODO(hiroshige): Turn these CHECK()s into DCHECK() before going to beta.
    ASSERT(nullptr != GetElement());
    ASSERT(m_isExternal == (nullptr != GetResource()));
    ASSERT(nullptr != GetResource() || !m_streamer);
}

std::unique_ptr<ClassicPendingScript> ClassicPendingScript::CreateInline(
    ScriptElementBase *element,
    const TextPosition &startingPosition,
    ScriptSourceLocationType sourceLocationType)
{
    std::unique_ptr<ClassicPendingScript> pendingScript = base::WrapUnique(new ClassicPendingScript(element, startingPosition, sourceLocationType, false));
    pendingScript->CheckState();
    return pendingScript;
}

void ClassicPendingScript::DataReceived(Resource *resource, const char *data, size_t size)
{
    ASSERT(false); // BKTODO:
}

void ClassicPendingScript::DisposeInternal(void)
{
    ClearResource();
    m_integrityFailure = false;
    CancelStreaming();
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
    ASSERT(false); // BKTODO:
    return nullptr;
#if 0
    ScriptResource *resource = ToScriptResource(GetResource());

    // If the MIME check fails, which is considered as load failure.
    if (!AllowedByNosniff::MimeTypeAsScript(
        GetElement()->GetDocument().ContextDocument(),
        resource->GetResponse())) {
        return nullptr;
    }

    // Check if we can use the script streamer.
    bool streamer_ready = false;
    ScriptStreamer::NotStreamingReason not_streamed_reason = not_streamed_reason_;
    if (streamer_) {
        DCHECK_EQ(not_streamed_reason, ScriptStreamer::kInvalid);
        if (streamer_->StreamingSuppressed()) {
            not_streamed_reason = streamer_->StreamingSuppressedReason();
        }
        else if (ready_state_ == kErrorOccurred) {
            not_streamed_reason = ScriptStreamer::kErrorOccurred;
        }
        else if (ready_state_ == kReadyStreaming) {
            not_streamed_reason = ScriptStreamer::kStreamerNotReadyOnGetSource;
        }
        else {
            // Streamer can be used to compile script.
            DCHECK_EQ(ready_state_, kReady);
            streamer_ready = true;
        }
    }
    RecordStreamingHistogram(GetSchedulingType(), streamer_ready,
        not_streamed_reason);

    ScriptSourceCode source_code(streamer_ready ? streamer_ : nullptr, resource,
        not_streamed_reason);
    // The base URL for external classic script is
    // "the URL from which the script was obtained" [spec text]
    // https://html.spec.whatwg.org/multipage/webappapis.html#concept-script-base-url
    const KURL& base_url = source_code.Url();
    return ClassicScript::Create(source_code, base_url, options_,
        resource->CalculateAccessControlStatus());
#endif
}

bool ClassicPendingScript::IsReady(void) const
{
    CheckState();
    return m_readyState >= kReady;
}

void ClassicPendingScript::NotifyFinished(Resource *resource)
{
    ASSERT(false); // BKTODO:
}

bool ClassicPendingScript::StartStreamingIfPossible(const std::function<void()> &done)
{
    ASSERT(false); // BKTODO:
    return false;
}

bool ClassicPendingScript::WasCanceled(void) const
{
    if (!m_isExternal)
        return false;
    return GetResource()->WasCanceled();
}

} // namespace blink
