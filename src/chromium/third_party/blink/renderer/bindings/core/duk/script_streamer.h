// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: script_streamer.h
// Description: ScriptCompiler Class
//      Author: Ziming Li
//     Created: 2020-02-06
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BLINKIT_BLINK_SCRIPT_STREAMER_H
#define BLINKIT_BLINK_SCRIPT_STREAMER_H

#pragma once

#include <string>
#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/wtf/noncopyable.h"

namespace base {
class SingleThreadTaskRunner;
}

namespace blink {

class ClassicPendingScript;
class ScriptResource;

// ScriptStreamer streams incomplete script data to V8 so that it can be parsed
// while it's loaded. ClassicPendingScript holds a reference to ScriptStreamer.
// At the moment, ScriptStreamer is only used for parser blocking scripts; this
// means that the Document stays stable and no other scripts are executing
// while we're streaming. It is possible, though, that Document and the
// ClassicPendingScript are destroyed while the streaming is in progress, and
// ScriptStreamer handles it gracefully.
class ScriptStreamer final : public GarbageCollectedFinalized<ScriptStreamer>
{
    WTF_MAKE_NONCOPYABLE(ScriptStreamer);
public:
    BK_DECLARE_GC_NAME(ScriptStreamer)

    // For tracking why some scripts are not streamed. Not streaming is part of
    // normal operation (e.g., script already loaded, script too small) and
    // doesn't necessarily indicate a failure.
    enum NotStreamingReason {
        kAlreadyLoaded,  // DEPRECATED
        kNotHTTP,
        kReload,
        kContextNotValid,  // DEPRECATED
        kEncodingNotSupported,
        kThreadBusy,
        kV8CannotStream,
        kScriptTooSmall,
        kNoResourceBuffer,
        kHasCodeCache,
        kStreamerNotReadyOnGetSource,
        kInlineScript,
        kDidntTryToStartStreaming,
        kErrorOccurred,

        // Pseudo values that should never be seen in reported metrics
        kCount,
        kInvalid = -1,
    };

    ~ScriptStreamer(void);
    void Trace(Visitor *visitor);

    // Launches a task (on a background thread) which will stream the given
    // ClassicPendingScript into V8 as it loads.
    static void StartStreaming(ClassicPendingScript *script, std::shared_ptr<base::SingleThreadTaskRunner> &loadingTaskRunner,
        NotStreamingReason *notStreamingReason);

    bool IsFinished(void) const;            // Has loading & streaming finished?
    bool IsStreamingFinished(void) const;   // Has streaming finished?

    // Called when the script is not needed any more (e.g., loading was
    // cancelled). After calling cancel, ClassicPendingScript can drop its
    // reference to ScriptStreamer, and ScriptStreamer takes care of eventually
    // deleting itself (after the V8 side has finished too).
    void Cancel(void);

    // When the streaming is suppressed, the data is not given to V8, but
    // ScriptStreamer still watches the resource load and notifies the upper
    // layers when loading is finished. It is used in situations when we have
    // started streaming but then we detect we don't want to stream (e.g., when
    // we have the code cache for the script) and we still want to parse and
    // execute it when it has finished loading.
    void SuppressStreaming(NotStreamingReason reason);
    bool StreamingSuppressed(void) const
    {
        ASSERT(!m_streamingSuppressed || m_suppressedReason != kInvalid);
        return m_streamingSuppressed;
    }
    NotStreamingReason StreamingSuppressedReason(void) const
    {
        ASSERT(m_streamingSuppressed || m_suppressedReason == kInvalid);
        return m_suppressedReason;
    }

    // Called by ClassicPendingScript when data arrives from the network.
    void NotifyAppendData(ScriptResource *resource);
    void NotifyFinished(void);
private:
    ScriptStreamer(ClassicPendingScript *script, std::shared_ptr<base::SingleThreadTaskRunner> &loadingTaskRunner);

    void NotifyFinishedToClient(void);
    bool HasEnoughDataForStreaming(size_t resourceBufferSize);

    // Scripts whose first data chunk is smaller than this constant won't be
    // streamed. Non-const for testing.
    static size_t m_smallScriptThreshold;
    // Maximum size of the BOM marker.
    static constexpr size_t kMaximumLengthOfBOM = 4;

    Member<ClassicPendingScript> m_pendingScript;

    bool m_loadingFinished = false;  // Whether loading from the network is done.
    bool m_parsingFinished = false;  // Whether the V8 side processing is done.
    // Whether we have received enough data to start the streaming.
    bool m_haveEnoughDataForStreaming = false;

    // Whether the script source code should be retrieved from the Resource
    // instead of the ScriptStreamer.
    bool m_streamingSuppressed = false;
    NotStreamingReason m_suppressedReason = kInvalid;

    // Encoding of the streamed script. Saved for sanity checking purposes.
    // BKTODO: std::string m_encoding;

    std::shared_ptr<base::SingleThreadTaskRunner> m_loadingTaskRunner;
};

} // namespace blink

#endif // BLINKIT_BLINK_SCRIPT_STREAMER_H
