// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: local_dom_window.h
// Description: LocalDOMWindow Class
//      Author: Ziming Li
//     Created: 2019-09-16
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2006, 2007, 2009, 2010 Apple Inc. All rights reserved.
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies)
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
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef BLINKIT_BLINK_LOCAL_DOM_WINDOW_H
#define BLINKIT_BLINK_LOCAL_DOM_WINDOW_H

#include <unordered_map>
#include <unordered_set>
#include "third_party/blink/renderer/core/frame/dom_window.h"
#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/wtf/assertions.h"

namespace BlinKit {
class DukTimer;
}

namespace blink {

class Document;
class DocumentInit;
class LocalFrame;
class Navigator;
#ifndef BLINKIT_CRAWLER_ONLY
class DOMVisualViewport;

enum PageshowEventPersistence {
    kPageshowEventNotPersisted = 0,
    kPageshowEventPersisted = 1
};
#endif

class LocalDOMWindow final : public DOMWindow
{
public:
    static LocalDOMWindow* Create(LocalFrame &frame)
    {
        return new LocalDOMWindow(frame);
    }
    ~LocalDOMWindow(void) override;
    void Trace(Visitor *visitor) override;

    // Exports for JS
    unsigned AddTimer(std::unique_ptr<BlinKit::DukTimer> &timer);
    Navigator* navigator(void) const;

    LocalFrame* GetFrame(void) const;
    Document* document(void) const { return m_document.Get(); }
    ExecutionContext* GetExecutionContext(void) const final;
#ifndef BLINKIT_CRAWLER_ONLY
    DOMVisualViewport* visualViewport(void);
#endif

    void Reset(void);

    Document* InstallNewDocument(const DocumentInit &init);

    DispatchEventResult DispatchEvent(Event &event, EventTarget *target);
    void DispatchWindowLoadEvent(void);

#ifndef BLINKIT_CRAWLER_ONLY
    void EnqueuePageshowEvent(PageshowEventPersistence persisted);
#endif

    void DocumentWasClosed(void);
    void FinishedLoading(void);
    void FrameDestroyed(void);

    class EventListenerObserver : public GarbageCollectedMixin {
    public:
        virtual void DidAddEventListener(LocalDOMWindow *, const AtomicString &) = 0;
        virtual void DidRemoveAllEventListeners(LocalDOMWindow *) = 0;
    };

    // EventTarget overrides
    void RemoveAllEventListeners(void) final;
protected:
    // EventTarget overrides
    void AddedEventListener(const AtomicString &eventType, RegisteredEventListener &registeredListener) override;
private:
    explicit LocalDOMWindow(LocalFrame &frame);

    void DispatchLoadEvent(void);
    void ClearDocument(void);
    void LaunchTimer(unsigned id, unsigned delayInMs);
    void ProcessTimer(unsigned id);

    const LocalDOMWindow* ToLocalDOMWindow(void) const final { return this; }

    Member<Document> m_document;

    mutable Member<Navigator> m_navigator;

    std::unordered_set<EventListenerObserver *> m_eventListenerObservers;

    unsigned m_nextTimerId = 1;
    std::unordered_map<unsigned, std::unique_ptr<BlinKit::DukTimer>> m_timers;

#ifndef BLINKIT_CRAWLER_ONLY
    Member<DOMVisualViewport> m_visualViewport;
#endif
};

DEFINE_TYPE_CASTS(LocalDOMWindow, DOMWindow, x, x->IsLocalDOMWindow(), x.IsLocalDOMWindow());

}  // namespace blink

#endif  // BLINKIT_BLINK_LOCAL_DOM_WINDOW_H
