// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: script_streamer.cpp
// Description: ScriptCompiler Class
//      Author: Ziming Li
//     Created: 2020-02-06
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "script_streamer.h"

#include "third_party/blink/renderer/core/script/classic_pending_script.h"
#include "third_party/blink/renderer/platform/shared_buffer.h"
#include "third_party/blink/renderer/platform/wtf/wtf.h"

namespace blink {

size_t ScriptStreamer::m_smallScriptThreshold = 30 * 1024;

ScriptStreamer::ScriptStreamer(ClassicPendingScript *script, std::shared_ptr<base::SingleThreadTaskRunner> &loadingTaskRunner)
    : m_pendingScript(script)
    // BKTODO:, m_encoding("UTF-8")
    , m_loadingTaskRunner(loadingTaskRunner)
{
}

ScriptStreamer::~ScriptStreamer(void) = default;

void ScriptStreamer::Cancel(void)
{
    ASSERT(IsMainThread());
}

bool ScriptStreamer::HasEnoughDataForStreaming(size_t resourceBufferSize)
{
    // Only stream larger scripts.
    return resourceBufferSize >= m_smallScriptThreshold;
}

bool ScriptStreamer::IsFinished(void) const
{
    ASSERT(IsMainThread());
    return m_loadingFinished && (m_parsingFinished || m_streamingSuppressed);
}

bool ScriptStreamer::IsStreamingFinished(void) const
{
    ASSERT(IsMainThread());
    return m_parsingFinished || m_streamingSuppressed;
}

void ScriptStreamer::NotifyAppendData(ScriptResource *resource)
{
    ASSERT(IsMainThread());
    if (m_streamingSuppressed)
        return;
    if (!m_haveEnoughDataForStreaming)
    {
        // Even if the first data chunk is small, the script can still be big
        // enough - wait until the next data chunk comes before deciding whether
        // to start the streaming.
        ASSERT(resource->ResourceBuffer());
        if (!HasEnoughDataForStreaming(resource->ResourceBuffer()->size()))
            return;
        m_haveEnoughDataForStreaming = true;

#if 0 // BKTODO: Check if necessary.
        {
            // Check for BOM (byte order marks), because that might change our
            // understanding of the data encoding.
            char maybeBom[kMaximumLengthOfBOM] = {};
            if (!resource->ResourceBuffer()->GetBytes(maybeBom, kMaximumLengthOfBOM))
            {
                NOTREACHED();
                return;
            }

            TextResourceDecoderOptions options(TextResourceDecoderOptions::kPlainTextContent, WTF::TextEncoding(resource->Encoding()));
            std::unique_ptr<TextResourceDecoder> decoder = TextResourceDecoder::Create(options);
            decoder->CheckForBOM(maybeBom, kMaximumLengthOfBOM);

            const char *encoding = decoder->Encoding().GetName();
            if (nullptr != encoding)
                m_encoding.assign(encoding);
        }
#endif

        m_parsingFinished = true;
#if 0 // BKTODO: Check if necessary.
        if (!RuntimeEnabledFeatures::ScheduledScriptStreamingEnabled() &&
            ScriptStreamerThread::Shared()->IsRunningTask()) {
            // If scheduled script streaming is disabled, we only have one thread for
            // running the tasks. A new task shouldn't be queued before the running
            // task completes, because the running task can block and wait for data
            // from the network.
            SuppressStreaming(kThreadBusy);
            return;
        }

        ASSERT(!m_stream);
        ASSERT(!m_source);
        m_stream = new SourceStream;
        // m_source takes ownership of m_stream.
        source_ = std::make_unique<v8::ScriptCompiler::StreamedSource>(stream_,
            encoding_);

        std::unique_ptr<v8::ScriptCompiler::ScriptStreamingTask>
            script_streaming_task(
                base::WrapUnique(v8::ScriptCompiler::StartStreamingScript(
                    V8PerIsolateData::MainThreadIsolate(), source_.get(),
                    compile_options_)));
        if (!script_streaming_task) {
            // V8 cannot stream the script.
            SuppressStreaming(kV8CannotStream);
            stream_ = nullptr;
            source_.reset();
            return;
        }

        if (RuntimeEnabledFeatures::ScheduledScriptStreamingEnabled()) {
            // Script streaming tasks are high priority, as they can block the parser,
            // and they can (and probably will) block during their own execution as
            // they wait for more input.
            //
            // Pass through the atomic cancellation token which is set to true by the
            // task when it is started, or set to true by the streamer if it wants to
            // cancel the task.
            //
            // TODO(leszeks): Decrease the priority of these tasks where possible.
            BackgroundScheduler::PostOnBackgroundThreadWithTraits(
                FROM_HERE, { base::TaskPriority::USER_BLOCKING, base::MayBlock() },
                CrossThreadBind(RunBlockingScriptStreamingTask,
                    WTF::Passed(std::move(script_streaming_task)),
                    WrapCrossThreadPersistent(this),
                    WTF::CrossThreadUnretained(
                        &blocking_task_started_or_cancelled_)));
        }
        else {
            blocking_task_started_or_cancelled_.test_and_set();
            ScriptStreamerThread::Shared()->PostTask(
                CrossThreadBind(&ScriptStreamerThread::RunScriptStreamingTask,
                    WTF::Passed(std::move(script_streaming_task)),
                    WrapCrossThreadPersistent(this)));
        }
#endif
    }
#if 0 // BKTODO: Check if necessary.
    if (m_stream)
        m_stream->DidReceiveData(resource, this);
#endif
}

void ScriptStreamer::NotifyFinished(void)
{
    ASSERT(IsMainThread());
    // A special case: empty and small scripts. We didn't receive enough data to
    // start the streaming before this notification. In that case, there won't
    // be a "parsing complete" notification either, and we should not wait for
    // it.
    if (!m_haveEnoughDataForStreaming)
        SuppressStreaming(kScriptTooSmall);

#if 0 // BKTODO: Check if necessary
    if (stream_) {
        // Mark the stream as finished loading before potentially re-posting the
        // task to avoid a race between this finish and the task's first read.
        stream_->DidFinishLoading();

        // If the corresponding blocking task hasn't started yet, cancel it and post
        // a non-blocking task, since we know now that all the data is received and
        // we will no longer block.
        //
        // TODO(874080): Once we have mutable task traits, simply unmark the
        // existing task as no longer MayBlock.
        if (RuntimeEnabledFeatures::ScheduledScriptStreamingEnabled() &&
            !blocking_task_started_or_cancelled_.test_and_set()) {
            std::unique_ptr<v8::ScriptCompiler::ScriptStreamingTask>
                script_streaming_task(
                    base::WrapUnique(v8::ScriptCompiler::StartStreamingScript(
                        V8PerIsolateData::MainThreadIsolate(), source_.get(),
                        compile_options_)));

            // The task creation shouldn't fail, since it didn't fail before during
            // NotifyAppendData.
            CHECK(script_streaming_task);
            BackgroundScheduler::PostOnBackgroundThreadWithTraits(
                FROM_HERE, { base::TaskPriority::USER_BLOCKING },
                CrossThreadBind(RunNonBlockingScriptStreamingTask,
                    WTF::Passed(std::move(script_streaming_task)),
                    WrapCrossThreadPersistent(this)));
        }
    }
#endif
    m_loadingFinished = true;

    NotifyFinishedToClient();
}

void ScriptStreamer::NotifyFinishedToClient(void)
{
    ASSERT(IsMainThread());
    // Usually, the loading will be finished first, and V8 will still need some
    // time to catch up. But the other way is possible too: if V8 detects a
    // parse error, the V8 side can complete before loading has finished. Send
    // the notification after both loading and V8 side operations have
    // completed.
    if (!IsFinished())
        return;

    m_pendingScript->StreamingFinished();
}

void ScriptStreamer::StartStreaming(
    ClassicPendingScript *script,
    std::shared_ptr<base::SingleThreadTaskRunner> &loadingTaskRunner,
    NotStreamingReason *notStreamingReason)
{
    DCHECK(IsMainThread());
    *notStreamingReason = kInvalid;
    ScriptResource *resource = ToScriptResource(script->GetResource());
    if (!resource->Url().SchemeIsHTTPOrHTTPS())
    {
        *notStreamingReason = kNotHTTP;
        return;
    }
    if (resource->IsLoaded() && !resource->ResourceBuffer())
    {
        // This happens for already loaded resources, e.g. if resource
        // validation fails. In that case, the loading subsystem will discard
        // the resource buffer.
        *notStreamingReason = kNoResourceBuffer;
        return;
    }
    // We cannot filter out short scripts, even if we wait for the HTTP headers
    // to arrive: the Content-Length HTTP header is not sent for chunked
    // downloads.

    ScriptStreamer *streamer = new ScriptStreamer(script, loadingTaskRunner);

    // If this script was ready when streaming began, no callbacks will be
    // received to populate the data for the ScriptStreamer, so send them now.
    // Note that this script may be processing an asynchronous cache hit, in
    // which case ScriptResource::IsLoaded() will be true, but ready_state_ will
    // not be kReadyStreaming. In that case, ScriptStreamer can listen to the
    // async callbacks generated by the cache hit.
    if (script->IsReady())
    {
        ASSERT(false); // BKTODO:
#if 0
        ASSERT(resource->IsLoaded());
        streamer->NotifyAppendData(resource);
        if (streamer->StreamingSuppressed())
        {
            *not_streaming_reason = streamer->StreamingSuppressedReason();
            return;
        }
#endif
    }

    // The Resource might go out of scope if the script is no longer needed.
    // This makes ClassicPendingScript notify the ScriptStreamer when it is
    // destroyed.
    script->SetStreamer(streamer);

    if (script->IsReady())
        streamer->NotifyFinished();
}

void ScriptStreamer::SuppressStreaming(NotStreamingReason reason)
{
    ASSERT(IsMainThread());
    ASSERT(!m_loadingFinished);
    ASSERT(NotStreamingReason::kInvalid != reason);

    // It can be that the parsing task has already finished (e.g., if there was
    // a parse error).
    m_streamingSuppressed = true;
    m_suppressedReason = reason;
}

void ScriptStreamer::Trace(Visitor *visitor)
{
    visitor->Trace(m_pendingScript);
}

} // namespace blink
