// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: local_frame.cpp
// Description: LocalFrame Class
//      Author: Ziming Li
//     Created: 2019-09-12
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1998, 1999 Torben Weis <weis@kde.org>
 *                     1999 Lars Knoll <knoll@kde.org>
 *                     1999 Antti Koivisto <koivisto@kde.org>
 *                     2000 Simon Hausmann <hausmann@kde.org>
 *                     2000 Stefan Schimanski <1Stein@gmx.de>
 *                     2001 George Staikos <staikos@kde.org>
 * Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011 Apple Inc. All
 * rights reserved.
 * Copyright (C) 2005 Alexey Proskuryakov <ap@nypop.com>
 * Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies)
 * Copyright (C) 2008 Eric Seidel <eric@webkit.org>
 * Copyright (C) 2008 Google Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "local_frame.h"

#include "third_party/blink/renderer/bindings/core/duk/script_controller.h"
#include "third_party/blink/renderer/core/dom/ignore_opens_during_unload_count_incrementer.h"
#include "third_party/blink/renderer/core/frame/local_dom_window.h"
#include "third_party/blink/renderer/core/frame/local_frame_client.h"
#include "third_party/blink/renderer/core/loader/navigation_scheduler.h"
#include "third_party/blink/renderer/platform/bindings/script_forbidden_scope.h"
#include "third_party/blink/renderer/platform/scheduler/main_thread/frame_scheduler_impl.h"
#include "third_party/blink/renderer/platform/wtf/wtf.h"
#ifndef BLINKIT_CRAWLER_ONLY
#   include "third_party/blink/renderer/core/scroll/smooth_scroll_sequencer.h"
#endif

using namespace BlinKit;

namespace blink {

static std::unique_ptr<FrameScheduler> CreateFrameScheduler(Page *)
{
    return scheduler::FrameSchedulerImpl::Create();
}

LocalFrame::LocalFrame(LocalFrameClient *client, Page *page)
    : Frame(client, page)
    , m_frameScheduler(CreateFrameScheduler(page))
    , m_loader(this)
    , m_navigationScheduler(NavigationScheduler::Create(this))
    , m_scriptController(ScriptController::Create(*this))
{
}

LocalFrame::~LocalFrame(void)
{
#ifndef BLINKIT_CRAWLER_ONLY
    ASSERT(!m_view);
#endif
}

LocalFrameClient* LocalFrame::Client(void) const
{
    return static_cast<LocalFrameClient *>(Frame::Client());
}

#ifndef BLINKIT_CRAWLER_ONLY
LayoutView* LocalFrame::ContentLayoutObject(void) const
{
    if (Document *document = GetDocument())
        return document->GetLayoutView();
    return nullptr;
}
#endif

std::unique_ptr<LocalFrame> LocalFrame::Create(LocalFrameClient *client, Page *page)
{
    return base::WrapUnique(new (ObjectType::Owner) LocalFrame(client, page));
}

#ifndef BLINKIT_CRAWLER_ONLY
void LocalFrame::CreateView(const IntSize &viewportSize, const Color &backgroundColor)
{
    ASSERT(nullptr != this);
    ASSERT(nullptr != GetPage());

#if 0 // BKTODO: Check if necessary
    bool is_local_root = this->IsLocalRoot();

    if (is_local_root && View())
        View()->SetParentVisible(false);
#endif

    std::shared_ptr<LocalFrameView> previousView(m_view);
    SetView(nullptr);

    std::shared_ptr<LocalFrameView> frameView = LocalFrameView::Create(*this, viewportSize);
    // The layout size is set by WebViewImpl to support @viewport
    frameView->SetLayoutSizeFixedToFrameSize(false);

    SetView(frameView);

    frameView->UpdateBaseBackgroundColorRecursively(backgroundColor);

#if 0 // BKTODO: Check if necessary
    if (is_local_root)
        frame_view->SetParentVisible(true);

    // FIXME: Not clear what the right thing for OOPI is here.
    if (OwnerLayoutObject()) {
        HTMLFrameOwnerElement* owner = DeprecatedLocalOwner();
        DCHECK(owner);
        // FIXME: OOPI might lead to us temporarily lying to a frame and telling it
        // that it's owned by a FrameOwner that knows nothing about it. If we're
        // lying to this frame, don't let it clobber the existing
        // EmbeddedContentView.
        if (owner->ContentFrame() == this)
            owner->SetEmbeddedContentView(frame_view);
    }

    if (Owner())
        View()->SetCanHaveScrollbars(Owner()->ScrollingMode() !=
            kScrollbarAlwaysOff);
#endif
}
#endif

void LocalFrame::DetachImpl(FrameDetachType type)
{
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // BEGIN RE-ENTRANCY SAFE BLOCK
    // Starting here, the code must be safe against re-entrancy. Dispatching
    // events, et cetera can run Javascript, which can reenter Detach().
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    m_loader.StopAllLoaders();
    // https://html.spec.whatwg.org/C/browsing-the-web.html#unload-a-document
    // The ignore-opens-during-unload counter of a Document must be incremented
    // both when unloading itself and when unloading its descendants.
    IgnoreOpensDuringUnloadCountIncrementer ignoreOpensDuringUnload(GetDocument());
    m_loader.DispatchUnloadEvent();

    // stopAllLoaders() needs to be called after detachChildren(), because
    // detachChildren() will trigger the unload event handlers of any child
    // frames, and those event handlers might start a new subresource load in this
    // frame.
    m_loader.StopAllLoaders();
    m_loader.Detach();
    GetDocument()->Shutdown();
#ifndef BLINKIT_CRAWLER_ONLY
    // TODO(crbug.com/729196): Trace why LocalFrameView::DetachFromLayout crashes.
    // It seems to crash because Frame is detached before LocalFrameView.
    // Verify here that any LocalFrameView has been detached by now.
    if (m_view && m_view->IsAttached())
    {
        ASSERT(false); // BKTODO: Check if necessary.
#if 0
        CHECK(DeprecatedLocalOwner());
        CHECK(DeprecatedLocalOwner()->OwnedEmbeddedContentView());
        CHECK_EQ(view_, DeprecatedLocalOwner()->OwnedEmbeddedContentView());
#endif
    }
    ASSERT(!m_view || !m_view->IsAttached());
#endif

    // This is the earliest that scripting can be disabled:
    // - FrameLoader::Detach() can fire XHR abort events
    // - Document::Shutdown() can dispose plugins which can run script.
    ScriptForbiddenScope forbidScript;
    if (nullptr == Client())
        return;

    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // END RE-ENTRANCY SAFE BLOCK
    // Past this point, no script should be executed. If this method was
    // re-entered, then check for a non-null Client() above should have already
    // returned.
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    ASSERT(!IsDetached());

#ifndef BLINKIT_CRAWLER_ONLY
    // TODO(crbug.com/729196): Trace why LocalFrameView::DetachFromLayout crashes.
    ASSERT(Client()->IsCrawler()); // BKTODO: CHECK(!view_->IsAttached());
#endif
    Client()->WillBeDetached();
    // Notify ScriptController that the frame is closing, since its cleanup ends
    // up calling back to LocalFrameClient via WindowProxy.
    GetScriptController().ClearForClose();

#ifndef BLINKIT_CRAWLER_ONLY
    ASSERT(Client()->IsCrawler()); // BKTODO:
#if 0
    // TODO(crbug.com/729196): Trace why LocalFrameView::DetachFromLayout crashes.
    CHECK(!view_->IsAttached());
    SetView(nullptr);
#endif
#endif

#if 0 // BKTODO:
    GetEventHandlerRegistry().DidRemoveAllEventHandlers(*DomWindow());
#endif

    DomWindow()->FrameDestroyed();

#ifndef BLINKIT_CRAWLER_ONLY
    ASSERT(Client()->IsCrawler()); // BKTODO:
#if 0
    if (GetPage() && GetPage()->GetFocusController().FocusedFrame() == this)
        GetPage()->GetFocusController().SetFocusedFrame(nullptr);
#endif
#endif

#if 0 // BKTODO:
    supplements_.clear();
#endif
    m_frameScheduler.reset();
#if 0 // BKTODO: Check the code below.
    WeakIdentifierMap<LocalFrame>::NotifyObjectDestroyed(this);
#endif
}

void LocalFrame::DocumentAttached(void)
{
    ASSERT(nullptr != GetDocument());
#ifndef BLINKIT_CRAWLER_ONLY
    if (Client()->IsCrawler())
        return;

    Document *document = GetDocument();
#if 0 // BKTODO:
    GetEditor().Clear();
    GetEventHandler().Clear();
    Selection().DocumentAttached(GetDocument());
    GetInputMethodController().DocumentAttached(GetDocument());
    GetSpellChecker().DocumentAttached(GetDocument());
    GetTextSuggestionController().DocumentAttached(GetDocument());
    previews_resource_loading_hints_receiver_.reset();
#endif
#endif
}

LocalDOMWindow* LocalFrame::DomWindow(void) const
{
    DOMWindow *domWindow = Frame::DomWindow();
    return ToLocalDOMWindow(domWindow);
}

Document* LocalFrame::GetDocument(void) const
{
    LocalDOMWindow *domWindow = DomWindow();
    return nullptr != domWindow ? domWindow->document() : nullptr;
}

#ifndef BLINKIT_CRAWLER_ONLY
SmoothScrollSequencer& LocalFrame::GetSmoothScrollSequencer(void)
{
    ASSERT(!Client()->IsCrawler());
    if (!m_smoothScrollSequencer)
        m_smoothScrollSequencer = std::make_unique<SmoothScrollSequencer>();
    return *m_smoothScrollSequencer;
}
#endif

std::shared_ptr<base::SingleThreadTaskRunner> LocalFrame::GetTaskRunner(TaskType type)
{
    ASSERT(IsMainThread());
    return m_frameScheduler->GetTaskRunner(type);
}

void LocalFrame::SetDOMWindow(std::unique_ptr<LocalDOMWindow> &domWindow)
{
    if (domWindow)
        GetScriptController().ClearWindowProxy();

    if (LocalDOMWindow *currentDomWindow = DomWindow())
        currentDomWindow->Reset();
    m_domWindow = std::move(domWindow);
}

#ifndef BLINKIT_CRAWLER_ONLY
void LocalFrame::SetPageZoomFactor(float factor)
{
    ASSERT(false); // BKTODO:
}

void LocalFrame::SetTextZoomFactor(float factor)
{
    ASSERT(false); // BKTODO:
}

void LocalFrame::SetView(const std::shared_ptr<LocalFrameView> &view)
{
    ASSERT(!m_view || m_view != view);
    ASSERT(nullptr == GetDocument() || !GetDocument()->IsActive());

    if (m_view)
        m_view->WillBeRemovedFromFrame();
    m_view = view;
}
#endif

bool LocalFrame::ShouldReuseDefaultView(void) const
{
    return m_loader.StateMachine()->IsDisplayingInitialEmptyDocument();
}

void LocalFrame::Trace(Visitor *visitor)
{
    m_loader.Trace(visitor);
}

} // namespace blink
