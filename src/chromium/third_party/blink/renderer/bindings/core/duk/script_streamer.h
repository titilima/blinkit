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

#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/wtf/noncopyable.h"

namespace blink {

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
};

} // namespace blink

#endif // BLINKIT_BLINK_SCRIPT_STREAMER_H
