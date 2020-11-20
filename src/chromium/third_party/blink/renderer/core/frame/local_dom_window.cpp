// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: local_dom_window.cpp
// Description: LocalDOMWindow Class
//      Author: Ziming Li
//     Created: 2019-09-16
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2006, 2007, 2008, 2010 Apple Inc. All rights reserved.
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

#include "local_dom_window.h"

#include "base/single_thread_task_runner.h"
#include "blinkit/crawler/dom/crawler_document.h"
#include "blinkit/gc/gc_heap.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_timer.h"
#include "third_party/blink/renderer/bindings/core/duk/script_controller.h"
#include "third_party/blink/renderer/core/dom/document_init.h"
#include "third_party/blink/renderer/core/dom/events/event.h"
#include "third_party/blink/renderer/core/dom/events/event_dispatch_forbidden_scope.h"
#include "third_party/blink/renderer/core/event_type_names.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/frame/local_frame_client.h"
#include "third_party/blink/renderer/core/frame/navigator.h"
#ifndef BLINKIT_CRAWLER_ONLY
#   include "third_party/blink/renderer/core/frame/viewport_data.h"
#   include "third_party/blink/renderer/core/html/html_document.h"
#endif

using namespace BlinKit;

namespace blink {

LocalDOMWindow::LocalDOMWindow(LocalFrame &frame) : DOMWindow(frame)
{
}

LocalDOMWindow::~LocalDOMWindow(void) = default;

void LocalDOMWindow::AddedEventListener(const AtomicString &eventType, RegisteredEventListener &registeredListener)
{
    DOMWindow::AddedEventListener(eventType, registeredListener);
#if 0 // BKTODO: Check if necessary.
    if (auto* frame = GetFrame()) {
        frame->GetEventHandlerRegistry().DidAddEventHandler(
            *this, event_type, registered_listener.Options());
    }
#endif

    if (Document *document = this->document())
        document->AddListenerTypeIfNeeded(eventType, *this);

    for (auto *ob : m_eventListenerObservers)
        ob->DidAddEventListener(this, eventType);

#if 0 // BKTODO: Check if necessary.
    if (eventType == event_type_names::kUnload)
        TrackUnloadEventListener(this);
    else if (eventType == event_type_names::kBeforeunload)
        TrackBeforeUnloadEventListener(this);
#endif
}

unsigned LocalDOMWindow::AddTimer(std::unique_ptr<DukTimer> &timer)
{
    unsigned delayInMs = timer->Interval();

    unsigned id = m_nextTimerId++;
    m_timers[id] = std::move(timer);

    LaunchTimer(id, delayInMs);
    return id;
}

void LocalDOMWindow::ClearDocument(void)
{
    if (!m_document)
        return;

    ASSERT(!m_document->IsActive());

    m_document->ClearDOMWindow();
    m_document.reset(nullptr);
}

DispatchEventResult LocalDOMWindow::DispatchEvent(Event &event, EventTarget *target)
{
#if DCHECK_IS_ON()
    ASSERT(!EventDispatchForbiddenScope::IsEventDispatchForbidden());
#endif

    event.SetTrusted(true);
    event.SetTarget(target ? target : this);
    event.SetCurrentTarget(this);
    event.SetEventPhase(Event::kAtTarget);

    return FireEventListeners(event);
}

void LocalDOMWindow::DispatchLoadEvent(void)
{
    Event *loadEvent = Event::Create(event_type_names::kLoad);
    DispatchEvent(*loadEvent, document());
}

void LocalDOMWindow::DispatchWindowLoadEvent(void)
{
#if DCHECK_IS_ON()
    ASSERT(!EventDispatchForbiddenScope::IsEventDispatchForbidden());
#endif
    DispatchLoadEvent();
}

void LocalDOMWindow::DocumentWasClosed(void)
{
    DispatchWindowLoadEvent();
#ifndef BLINKIT_CRAWLER_ONLY
    if (!GetFrame()->Client()->IsCrawler())
    {
        ASSERT(false); // BKTODO:
#if 0
        EnqueuePageshowEvent(kPageshowEventNotPersisted);
        if (pending_state_object_)
            EnqueuePopstateEvent(std::move(pending_state_object_));
#endif
    }
#endif
}

void LocalDOMWindow::FinishedLoading(void)
{
    // Currently nothing to do.
}

void LocalDOMWindow::FrameDestroyed(void)
{
    m_timers.clear();
    RemoveAllEventListeners();
    DisconnectFromFrame();
}

ExecutionContext* LocalDOMWindow::GetExecutionContext(void) const
{
    return m_document.get();
}

LocalFrame* LocalDOMWindow::GetFrame(void) const
{
    return ToLocalFrame(DOMWindow::GetFrame());
}

Document* LocalDOMWindow::InstallNewDocument(const DocumentInit &init)
{
    LocalFrame *frame = GetFrame();
    ASSERT(init.GetFrame() == frame);

    ClearDocument();

#ifdef BLINKIT_CRAWLER_ONLY
    ASSERT(init.GetFrame()->Client()->IsCrawler());
    m_document = CrawlerDocument::Create(init);
#else
    const bool isCrawler = init.GetFrame()->Client()->IsCrawler();
    if (isCrawler)
        m_document = std::make_unique<CrawlerDocument>(init);
    else
        m_document = std::make_unique<HTMLDocument>(init);
#endif
    m_document->Initialize();

    if (nullptr != frame)
    {
        frame->GetScriptController().UpdateDocument();
#ifndef BLINKIT_CRAWLER_ONLY
        if (!isCrawler)
            m_document->GetViewportData().UpdateViewportDescription();
#endif
    }

    return m_document.get();
}

void LocalDOMWindow::LaunchTimer(unsigned id, unsigned delayInMs)
{
    if (auto taskRunner = GetFrame()->GetTaskRunner(TaskType::kJavascriptTimer))
    {
        auto callback = std::bind(&LocalDOMWindow::ProcessTimer, this, id);
        taskRunner->PostDelayedTask(FROM_HERE, callback, TimeDelta::FromMilliseconds(delayInMs));
    }
}

Navigator* LocalDOMWindow::navigator(void) const
{
    if (!m_navigator)
        m_navigator = Navigator::Create(GetFrame());
    return m_navigator.Get();
}

void LocalDOMWindow::ProcessTimer(unsigned id)
{
    auto it = m_timers.find(id);
    if (std::end(m_timers) == it)
        return;

    if (it->second->Fire() && it->second->IsRepeatable())
    {
        unsigned delayInMs = it->second->Interval();
        LaunchTimer(id, delayInMs);
    }
    else
    {
        m_timers.erase(it);
    }
}

void LocalDOMWindow::RemoveAllEventListeners(void)
{
    EventTarget::RemoveAllEventListeners();

    for (EventListenerObserver *observer : m_eventListenerObservers)
        observer->DidRemoveAllEventListeners(this);

#if 0 // BKTODO:
    if (LocalFrame *frame = GetFrame())
        frame->GetEventHandlerRegistry().DidRemoveAllEventHandlers(*this);

    UntrackAllUnloadEventListeners(this);
    UntrackAllBeforeUnloadEventListeners(this);
#endif
}

void LocalDOMWindow::Reset(void)
{
    ASSERT(nullptr != document());
    ASSERT(document()->IsContextDestroyed());
    FrameDestroyed();

    m_navigator = nullptr;
}

void LocalDOMWindow::Trace(Visitor *visitor)
{
    visitor->Trace(m_navigator);
    DOMWindow::Trace(visitor);
}

} // namespace blink
