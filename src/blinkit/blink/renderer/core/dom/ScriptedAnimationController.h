// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: ScriptedAnimationController.h
// Description: ScriptedAnimationController Class
//      Author: Ziming Li
//     Created: 2021-08-16
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2011 Google Inc. All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 *  THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS'' AND ANY
 *  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 *  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 *  ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef ScriptedAnimationController_h
#define ScriptedAnimationController_h

#include <unordered_set>
#include "core/dom/FrameRequestCallbackCollection.h"
#include "platform/heap/Handle.h"
#include "wtf/ListHashSet.h"
#include "wtf/RefCounted.h"
#include "wtf/RefPtr.h"
#include "wtf/Vector.h"
#include "wtf/text/AtomicString.h"
#include "wtf/text/StringImpl.h"

namespace blink {

class Document;
class Event;
class EventTarget;
class FrameRequestCallback;
class MediaQueryListListener;

class ScriptedAnimationController {
    DECLARE_EMPTY_DESTRUCTOR_WILL_BE_REMOVED(ScriptedAnimationController);
public:
    static GCUniquePtr<ScriptedAnimationController> create(Document* document)
    {
        return BlinKit::GCWrapUnique(new ScriptedAnimationController(document));
    }

    DECLARE_TRACE();
    void clearDocumentPointer() { m_document = nullptr; }

    typedef int CallbackId;

    int registerCallback(FrameRequestCallback*);
    void cancelCallback(CallbackId);
    void serviceScriptedAnimations(double monotonicTimeNow);

    void enqueueEvent(const GCRefPtr<Event> &);
    void enqueuePerFrameEvent(const GCRefPtr<Event> &);
    void enqueueMediaQueryChangeListeners(WillBeHeapVector<RefPtrWillBeMember<MediaQueryListListener>>&);

    void suspend();
    void resume();

    void dispatchEventsAndCallbacksForPrinting();
private:
    explicit ScriptedAnimationController(Document*);

    void scheduleAnimationIfNeeded();

    void dispatchEvents(const AtomicString& eventInterfaceFilter = AtomicString());
    void executeCallbacks(double monotonicTimeNow);
    void callMediaQueryListListeners();

    bool hasScheduledItems() const;

    Document *m_document;
    FrameRequestCallbackCollection m_callbackCollection;
    int m_suspendCount;
    std::vector<GCRefPtr<Event>> m_eventQueue;
    std::unordered_set<unsigned> m_perFrameEventHashes;
    using MediaQueryListListeners = WillBeHeapListHashSet<RefPtrWillBeMember<MediaQueryListListener>>;
    MediaQueryListListeners m_mediaQueryListListeners;
};

} // namespace blink

#endif // ScriptedAnimationController_h
