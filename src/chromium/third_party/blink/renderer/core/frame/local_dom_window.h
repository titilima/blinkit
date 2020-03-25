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

#include <unordered_set>
#include "base/memory/ptr_util.h"
#include "third_party/blink/renderer/core/frame/dom_window.h"
#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/wtf/assertions.h"

namespace blink {

class Document;
class DocumentInit;
class LocalFrame;

class LocalDOMWindow final : public DOMWindow
{
public:
    static std::unique_ptr<LocalDOMWindow> Create(LocalFrame &frame)
    {
        return base::WrapUnique(new LocalDOMWindow(frame));
    }
    ~LocalDOMWindow(void) override;

    LocalFrame* GetFrame(void) const;
    Document* document(void) const { return m_document.get(); }

    void Reset(void);

    Document* InstallNewDocument(const DocumentInit &init);

    DispatchEventResult DispatchEvent(Event &event, EventTarget *target);
    void DispatchWindowLoadEvent(void);

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

    ExecutionContext* GetExecutionContext(void) const final;
    const LocalDOMWindow* ToLocalDOMWindow(void) const final { return this; }

    std::unique_ptr<Document> m_document;

    std::unordered_set<EventListenerObserver *> m_eventListenerObservers;
};

DEFINE_TYPE_CASTS(LocalDOMWindow, DOMWindow, x, x->IsLocalDOMWindow(), x.IsLocalDOMWindow());

}  // namespace blink

#endif  // BLINKIT_BLINK_LOCAL_DOM_WINDOW_H
