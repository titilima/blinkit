// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: ScriptedAnimationController.cpp
// Description: ScriptedAnimationController Class
//      Author: Ziming Li
//     Created: 2021-07-21
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

#include "core/dom/ScriptedAnimationController.h"

#include "core/css/MediaQueryListListener.h"
#include "core/dom/Document.h"
#include "core/dom/FrameRequestCallback.h"
#include "core/events/Event.h"
#include "core/frame/FrameView.h"
#include "core/frame/LocalDOMWindow.h"
#include "core/inspector/InspectorInstrumentation.h"
#include "core/inspector/InspectorTraceEvents.h"
#include "core/loader/DocumentLoader.h"
// BKTODO: #include "platform/Logging.h"

using namespace BlinKit;

namespace blink {

static unsigned eventTargetKey(const Event *event)
{
    return WTF::pairIntHash(reinterpret_cast<unsigned>(event->target()), reinterpret_cast<unsigned>(event->type().impl()));
}

ScriptedAnimationController::ScriptedAnimationController(Document* document)
    : m_document(document)
    , m_callbackCollection(document)
    , m_suspendCount(0)
{
}

DEFINE_EMPTY_DESTRUCTOR_WILL_BE_REMOVED(ScriptedAnimationController);

DEFINE_TRACE(ScriptedAnimationController)
{
#if ENABLE(OILPAN)
    // BKTODO: visitor->trace(m_document);
    visitor->trace(m_callbackCollection);
    visitor->trace(m_eventQueue);
    visitor->trace(m_mediaQueryListListeners);
    // BKTODO: visitor->trace(m_perFrameEvents);
#endif
}

void ScriptedAnimationController::suspend()
{
    ++m_suspendCount;
}

void ScriptedAnimationController::resume()
{
    // It would be nice to put an ASSERT(m_suspendCount > 0) here, but in WK1 resume() can be called
    // even when suspend hasn't (if a tab was created in the background).
    if (m_suspendCount > 0)
        --m_suspendCount;
    scheduleAnimationIfNeeded();
}

void ScriptedAnimationController::dispatchEventsAndCallbacksForPrinting()
{
    dispatchEvents(EventNames::MediaQueryListEvent);
    callMediaQueryListListeners();
}

ScriptedAnimationController::CallbackId ScriptedAnimationController::registerCallback(FrameRequestCallback* callback)
{
    CallbackId id = m_callbackCollection.registerCallback(callback);
    scheduleAnimationIfNeeded();
    return id;
}

void ScriptedAnimationController::cancelCallback(CallbackId id)
{
    m_callbackCollection.cancelCallback(id);
}

void ScriptedAnimationController::dispatchEvents(const AtomicString& eventInterfaceFilter)
{
    std::vector<GCRefPtr<Event>> events;
    if (eventInterfaceFilter.isEmpty()) {
        events.swap(m_eventQueue);
        m_perFrameEventHashes.clear();
    } else {
        std::vector<GCRefPtr<Event>> remaining;
        for (auto& event : m_eventQueue) {
            if (event && event->interfaceName() == eventInterfaceFilter) {
                m_perFrameEventHashes.erase(eventTargetKey(event.get()));
                events.emplace_back(event.release());
            } else {
                remaining.emplace_back(event.release());
            }
        }
        remaining.swap(m_eventQueue);
    }


    for (size_t i = 0; i < events.size(); ++i) {
        EventTarget* eventTarget = events[i]->target();
        // FIXME: we should figure out how to make dispatchEvent properly virtual to avoid
        // special casting window.
        // FIXME: We should not fire events for nodes that are no longer in the tree.
        if (LocalDOMWindow* window = eventTarget->toDOMWindow())
            window->dispatchEvent(events[i], nullptr);
        else
            eventTarget->dispatchEvent(events[i]);

        InspectorInstrumentation::didRemoveEvent(eventTarget, events[i].get());
    }
}

void ScriptedAnimationController::executeCallbacks(double monotonicTimeNow)
{
    // dispatchEvents() runs script which can cause the document to be destroyed.
    if (!m_document)
        return;

    ASSERT(m_callbackCollection.isEmpty()); // BKTODO:
#if 0
    double highResNowMs = 1000.0 * m_document->loader()->timing().monotonicTimeToZeroBasedDocumentTime(monotonicTimeNow);
    double legacyHighResNowMs = 1000.0 * m_document->loader()->timing().monotonicTimeToPseudoWallTime(monotonicTimeNow);
    m_callbackCollection.executeCallbacks(highResNowMs, legacyHighResNowMs);
#endif
}

void ScriptedAnimationController::callMediaQueryListListeners()
{
    MediaQueryListListeners listeners;
    listeners.swap(m_mediaQueryListListeners);

    for (const auto& listener : listeners) {
        listener->notifyMediaQueryChanged();
    }
}

bool ScriptedAnimationController::hasScheduledItems() const
{
    if (m_suspendCount)
        return false;

    return !m_callbackCollection.isEmpty() || !m_eventQueue.empty() || !m_mediaQueryListListeners.isEmpty();
}

void ScriptedAnimationController::serviceScriptedAnimations(double monotonicTimeNow)
{
    if (!hasScheduledItems())
        return;

    RefPtrWillBeRawPtr<ScriptedAnimationController> protect(this);

    callMediaQueryListListeners();
    dispatchEvents();
    executeCallbacks(monotonicTimeNow);

    scheduleAnimationIfNeeded();
}

void ScriptedAnimationController::enqueueEvent(const GCRefPtr<Event> &event)
{
    InspectorInstrumentation::didEnqueueEvent(event->target(), event.get());
    m_eventQueue.emplace_back(event);
    scheduleAnimationIfNeeded();
}

void ScriptedAnimationController::enqueuePerFrameEvent(const GCRefPtr<Event> &event)
{
    unsigned key = eventTargetKey(event.get());
    if (zed::key_exists(m_perFrameEventHashes, key))
        return;
    m_perFrameEventHashes.emplace(key);
    enqueueEvent(event);
}

void ScriptedAnimationController::enqueueMediaQueryChangeListeners(WillBeHeapVector<RefPtrWillBeMember<MediaQueryListListener>>& listeners)
{
    for (size_t i = 0; i < listeners.size(); ++i) {
        ASSERT(false); // BKTODO: m_mediaQueryListListeners.add(listeners[i]);
    }
    scheduleAnimationIfNeeded();
}

void ScriptedAnimationController::scheduleAnimationIfNeeded()
{
    if (!hasScheduledItems())
        return;

    if (!m_document)
        return;

    if (FrameView* frameView = m_document->view())
        frameView->scheduleAnimation();
}

}
