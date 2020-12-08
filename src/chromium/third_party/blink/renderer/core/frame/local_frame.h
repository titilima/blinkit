// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: local_frame.h
// Description: LocalFrame Class
//      Author: Ziming Li
//     Created: 2019-09-12
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1998, 1999 Torben Weis <weis@kde.org>
 *                     1999-2001 Lars Knoll <knoll@kde.org>
 *                     1999-2001 Antti Koivisto <koivisto@kde.org>
 *                     2000-2001 Simon Hausmann <hausmann@kde.org>
 *                     2000-2001 Dirk Mueller <mueller@kde.org>
 *                     2000 Stefan Schimanski <1Stein@gmx.de>
 * Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009, 2010 Apple Inc. All rights
 * reserved.
 * Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies)
 * Copyright (C) 2008 Eric Seidel <eric@webkit.org>
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

#ifndef BLINKIT_BLINK_LOCAL_FRAME_H
#define BLINKIT_BLINK_LOCAL_FRAME_H

#pragma once

#include "third_party/blink/public/platform/task_type.h"
#include "third_party/blink/renderer/core/frame/frame.h"
#include "third_party/blink/renderer/core/loader/frame_loader.h"
#include "third_party/blink/renderer/platform/wtf/wtf.h"
#ifndef BLINKIT_CRAWLER_ONLY
#   include "third_party/blink/renderer/core/frame/local_frame_view.h"
#endif

namespace base {
class SingleThreadTaskRunner;
}

namespace blink {

class Document;
class FrameScheduler;
class LocalDOMWindow;
class LocalFrameClient;
class NavigationScheduler;
class ScriptController;
#ifndef BLINKIT_CRAWLER_ONLY
class FrameSelection;
class LayoutView;
class SmoothScrollSequencer;
#endif

class LocalFrame final : public Frame
{
public:
    static std::unique_ptr<LocalFrame> Create(LocalFrameClient *client, Page *page = nullptr);
    ~LocalFrame(void) override;
    void Trace(Visitor *visitor) override;

    void Init(void) { m_loader.Init(); }

    LocalFrameClient* Client(void) const;
    FrameLoader& Loader(void) const { return m_loader; }
    NavigationScheduler& GetNavigationScheduler(void) const { return *m_navigationScheduler; }
    LocalDOMWindow* DomWindow(void) const;
    void SetDOMWindow(LocalDOMWindow *domWindow);
#ifndef BLINKIT_CRAWLER_ONLY
    // Root of the layout tree for the document contained in this frame.
    LayoutView* ContentLayoutObject(void) const;
    LocalFrameView* View(void) const override { return m_view.Get(); }
    void CreateView(const IntSize &viewportSize, const Color &backgroundColor);
    void SetView(LocalFrameView *view);

    FrameSelection& Selection(void) const { return *m_selection; }

    double DevicePixelRatio(void) const;
    void DeviceScaleFactorChanged(void);

    float PageZoomFactor(void) const { return m_pageZoomFactor; }
    void SetPageZoomFactor(float factor);
    float TextZoomFactor(void) const { return m_textZoomFactor; }
    void SetTextZoomFactor(float factor);

    SmoothScrollSequencer& GetSmoothScrollSequencer(void);

    bool ShouldThrottleRendering(void) const;
    void ScheduleVisualUpdateUnlessThrottled(void);
#endif
    Document* GetDocument(void) const;
    std::shared_ptr<base::SingleThreadTaskRunner> GetTaskRunner(TaskType type);
    ScriptController& GetScriptController(void) const { return *m_scriptController; }

    bool IsNavigationAllowed(void) const { return 0 == m_navigationDisableCount; }

    bool ShouldReuseDefaultView(void) const;

    void DocumentAttached(void);
private:
    friend class FrameNavigationDisabler;

    LocalFrame(LocalFrameClient *client, Page *page);

    void EnableNavigation(void) { --m_navigationDisableCount; }
    void DisableNavigation(void) { ++m_navigationDisableCount; }

    // Frame overrides
    void DetachImpl(FrameDetachType type) override;

    std::unique_ptr<FrameScheduler> m_frameScheduler;
    mutable FrameLoader m_loader;
    std::unique_ptr<NavigationScheduler> m_navigationScheduler;

#ifndef BLINKIT_CRAWLER_ONLY
    // Cleared by LocalFrame::Detach(), so as to keep the observable lifespan
    // of LocalFrame::View().
    Member<LocalFrameView> m_view;
    std::unique_ptr<FrameSelection> m_selection;
    // SmoothScrollSequencer is only populated for local roots; all local frames
    // use the instance owned by their local root.
    std::unique_ptr<SmoothScrollSequencer> m_smoothScrollSequencer;

    float m_pageZoomFactor = 1.0;
    float m_textZoomFactor = 1.0;
#endif

    const std::unique_ptr<ScriptController> m_scriptController;

    int m_navigationDisableCount = 0;
};

DEFINE_TYPE_CASTS(LocalFrame, Frame, x, x->IsLocalFrame(), x.IsLocalFrame());

class FrameNavigationDisabler
{
    STACK_ALLOCATED();
public:
    explicit FrameNavigationDisabler(LocalFrame &frame) : m_frame(frame)
    {
        m_frame->DisableNavigation();
    }
    ~FrameNavigationDisabler(void)
    {
        m_frame->EnableNavigation();
    }
private:
    Member<LocalFrame> m_frame;

    DISALLOW_COPY_AND_ASSIGN(FrameNavigationDisabler);
};

} // namespace blink

#endif // BLINKIT_BLINK_LOCAL_FRAME_H
