// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: PointerLockController.cpp
// Description: PointerLockController Class
//      Author: Ziming Li
//     Created: 2021-07-27
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2012 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "./PointerLockController.h"

#include "blinkit/blink/renderer/core/dom/element.h"
#include "blinkit/blink/renderer/core/events/Event.h"
#include "blinkit/blink/renderer/core/frame/local_dom_window.h"
// BKTODO: #include "core/inspector/ConsoleMessage.h"
#include "blinkit/blink/renderer/core/page/ChromeClient.h"
#include "blinkit/blink/renderer/core/page/Page.h"
#include "blinkit/blink/renderer/platform/PlatformMouseEvent.h"

namespace blink {

PointerLockController::PointerLockController(Page* page)
    : m_page(page)
    , m_lockPending(false)
{
}

std::unique_ptr<PointerLockController> PointerLockController::create(Page* page)
{
    return zed::wrap_unique(new PointerLockController(page));
}

void PointerLockController::requestPointerLock(Element* target)
{
    ASSERT(false); // BKTODO:
#if 0
    if (!target || !target->inDocument() || m_documentOfRemovedElementWhileWaitingForUnlock) {
        enqueueEvent(EventTypeNames::pointerlockerror, target);
        return;
    }

    UseCounter::countCrossOriginIframe(target->document(), UseCounter::ElementRequestPointerLockIframe);

    if (target->document().isSandboxed(SandboxPointerLock)) {
        // FIXME: This message should be moved off the console once a solution to https://bugs.webkit.org/show_bug.cgi?id=103274 exists.
        target->document().addConsoleMessage(ConsoleMessage::create(SecurityMessageSource, ErrorMessageLevel, "Blocked pointer lock on an element because the element's frame is sandboxed and the 'allow-pointer-lock' permission is not set."));
        enqueueEvent(EventTypeNames::pointerlockerror, target);
        return;
    }

    if (m_element) {
        if (m_element->document() != target->document()) {
            enqueueEvent(EventTypeNames::pointerlockerror, target);
            return;
        }
        enqueueEvent(EventTypeNames::pointerlockchange, target);
        m_element = target;
    } else if (m_page->chromeClient().requestPointerLock()) {
        m_lockPending = true;
        m_element = target;
    } else {
        enqueueEvent(EventTypeNames::pointerlockerror, target);
    }
#endif
}

void PointerLockController::requestPointerUnlock()
{
    return m_page->chromeClient().requestPointerUnlock();
}

void PointerLockController::elementRemoved(Element* element)
{
    if (m_element == element) {
        m_documentOfRemovedElementWhileWaitingForUnlock = &m_element->document();
        // Set element null immediately to block any future interaction with it
        // including mouse events received before the unlock completes.
        clearElement();
        requestPointerUnlock();
    }
}

void PointerLockController::documentDetached(Document* document)
{
    if (m_element && m_element->document() == document) {
        clearElement();
        requestPointerUnlock();
    }
}

bool PointerLockController::lockPending() const
{
    return m_lockPending;
}

Element* PointerLockController::element() const
{
    return m_element.get();
}

void PointerLockController::didAcquirePointerLock()
{
    ASSERT(false); // BKTODO: enqueueEvent(EventTypeNames::pointerlockchange, m_element.get());
    m_lockPending = false;
}

void PointerLockController::didNotAcquirePointerLock()
{
    ASSERT(false); // BKTODO: enqueueEvent(EventTypeNames::pointerlockerror, m_element.get());
    clearElement();
}

void PointerLockController::didLosePointerLock()
{
    ASSERT(false); // BKTODO: enqueueEvent(EventTypeNames::pointerlockchange, m_element ? &m_element->document() : m_documentOfRemovedElementWhileWaitingForUnlock.get());
    clearElement();
    m_documentOfRemovedElementWhileWaitingForUnlock = nullptr;
}

void PointerLockController::dispatchLockedMouseEvent(const PlatformMouseEvent& event, const AtomicString& eventType)
{
    if (!m_element || !m_element->document().frame())
        return;

    m_element->dispatchMouseEvent(event, eventType, event.clickCount());

    // Create click events
    if (eventType == EventTypeNames::mouseup)
        m_element->dispatchMouseEvent(event, EventTypeNames::click, event.clickCount());
}

void PointerLockController::clearElement()
{
    m_lockPending = false;
    m_element = nullptr;
}

void PointerLockController::enqueueEvent(const AtomicString& type, Element* element)
{
    if (element)
        enqueueEvent(type, &element->document());
}

void PointerLockController::enqueueEvent(const AtomicString& type, Document* document)
{
    if (document && document->domWindow())
        document->domWindow()->enqueueDocumentEvent(Event::create(type));
}

} // namespace blink
