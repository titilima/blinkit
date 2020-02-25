// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: classic_pending_script.h
// Description: ClassicPendingScript Class
//      Author: Ziming Li
//     Created: 2020-02-02
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BLINKIT_BLINK_CLASSIC_PENDING_SCRIPT_H
#define BLINKIT_BLINK_CLASSIC_PENDING_SCRIPT_H

#pragma once

#include "third_party/blink/renderer/bindings/core/duk/script_streamer.h"
#include "third_party/blink/renderer/bindings/core/v8/script_source_location_type.h"
#include "third_party/blink/renderer/core/script/classic_script.h"
#include "third_party/blink/renderer/core/script/pending_script.h"
#include "third_party/blink/renderer/core/loader/resource/script_resource.h"

namespace blink {

// PendingScript for a classic script
// https://html.spec.whatwg.org/multipage/webappapis.html#classic-script.
//
// TODO(kochi): The comment below is from pre-oilpan age and may not be correct
// now.
// A RefPtr alone does not prevent the underlying Resource from purging its data
// buffer. This class holds a dummy client open for its lifetime in order to
// guarantee that the data buffer will not be purged.
class ClassicPendingScript final : public PendingScript, public ResourceClient
{
    USING_GARBAGE_COLLECTED_MIXIN(ClassicPendingScript);
public:
    // https://html.spec.whatwg.org/multipage/webappapis.html#fetch-a-classic-script
    //
    // For a script from an external file, calls ScriptResource::Fetch() and
    // creates ClassicPendingScript. Returns nullptr if Fetch() returns nullptr.
    static std::unique_ptr<ClassicPendingScript> Fetch(const BlinKit::BkURL &url, Document &elementDocument,
        const WTF::TextEncoding &encoding, ScriptElementBase *element);
    // For an inline script.
    static std::unique_ptr<ClassicPendingScript> CreateInline(ScriptElementBase *element,
        const TextPosition &startingPosition, ScriptSourceLocationType sourceLocationType);
    ~ClassicPendingScript(void) override;

    // ScriptStreamer callbacks.
    void SetStreamer(std::unique_ptr<ScriptStreamer> &streamer);
    void StreamingFinished(void);

    std::unique_ptr<Script> GetSource(const BlinKit::BkURL &documentURL) const override;
    ScriptType GetScriptType(void) const override { return ScriptType::kClassic; }
    bool IsReady(void) const override;
    bool IsExternal(void) const override { return m_isExternal; }
    bool WasCanceled(void) const override;
private:
    // See AdvanceReadyState implementation for valid state transitions.
    enum ReadyState {
        // These states are considered "not ready".
        kWaitingForResource,
        kWaitingForStreaming,
        // These states are considered "ready".
        kReady,
        kReadyStreaming,
        kErrorOccurred,
    };

    ClassicPendingScript(ScriptElementBase *element, const TextPosition &startingPosition,
        ScriptSourceLocationType sourceLocationType, bool isExternal);

    // Advances the current state of the script, reporting to the client if
    // appropriate.
    void AdvanceReadyState(ReadyState newReadyState);

    // Handle the end of streaming.
    void FinishWaitingForStreaming(void);
    void FinishReadyStreaming(void);
    void CancelStreaming(void);

    bool StartStreamingIfPossible(const std::function<void()> &done) override;
    bool IsCurrentlyStreaming(void) const override { return m_isCurrentlyStreaming; }
    void DisposeInternal(void) override;
    void CheckState(void) const override;
    // ResourceClient
    void NotifyFinished(Resource *resource) override;
    void DataReceived(Resource *resource, const char *data, size_t size) override;

    // "base url" snapshot taken at #prepare-a-script timing.
    // https://html.spec.whatwg.org/multipage/scripting.html#prepare-a-script
    // which will eventually be used as #concept-script-base-url.
    // https://html.spec.whatwg.org/multipage/webappapis.html#concept-script-base-url
    const BlinKit::BkURL m_baseUrlForInlineScript;

    // "element's child text content" snapshot taken at
    // #prepare-a-script (Step 4).
    const String m_sourceTextForInlineScript;

    const ScriptSourceLocationType m_sourceLocationType;
    const bool m_isExternal;
    ReadyState m_readyState;
    bool m_integrityFailure = false;

    std::unique_ptr<ScriptStreamer> m_streamer;
    std::function<void()> m_streamerDone;

    // This flag tracks whether streamer_ is currently streaming. It is used
    // mainly to prevent re-streaming a script while it is being streamed.
    //
    // ReadyState unfortunately doesn't contain this information, because
    // 1, the WaitingFor* states can occur with or without streaming, and
    // 2, during the state transition, we need to first transition ready_state_,
    //    then run callbacks, and only then consider the streaming done. So
    //    during AdvanceReadyState and callback processing, the ready state
    //    and is_currently_streaming_ are temporarily different. (They must
    //    be consistent before and after AdvanceReadyState.)
    //
    // (See also: crbug.com/754360)
    bool m_isCurrentlyStreaming = false;

    // Specifies the reason that script was never streamed.
    ScriptStreamer::NotStreamingReason m_notStreamedReason;
};

} // namespace blink

#endif // BLINKIT_BLINK_CLASSIC_PENDING_SCRIPT_H
