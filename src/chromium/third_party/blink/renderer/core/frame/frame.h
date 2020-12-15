// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: frame.h
// Description: Frame Class
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

#ifndef BLINKIT_BLINK_FRAME_H
#define BLINKIT_BLINK_FRAME_H

#pragma once

#include "third_party/blink/renderer/core/frame/frame_lifecycle.h"
#include "third_party/blink/renderer/platform/heap/handle.h"
#ifndef BLINKIT_CRAWLER_ONLY
#   include "third_party/blink/renderer/core/frame/frame_view.h"
#   include "third_party/blink/renderer/platform/graphics/touch_action.h"
#endif

namespace blink {

class DOMWindow;
class FrameClient;
#ifndef BLINKIT_CRAWLER_ONLY
class ChromeClient;
class Page;
#endif

enum class FrameDetachType { kRemove, kSwap };

class Frame : public GarbageCollectedFinalized<Frame>
{
public:
    BK_DECLARE_GC_NAME(Frame)

    virtual ~Frame(void);
    virtual void Trace(Visitor *visitor);

    FrameClient* Client(void) const { return m_client; }
    DOMWindow* DomWindow(void) const { return m_domWindow.Get(); }
#ifndef BLINKIT_CRAWLER_ONLY
    Page* GetPage(void) const;  // Null when the frame is detached.
    ChromeClient& GetChromeClient(void) const;
    virtual FrameView* View(void) const = 0;
#endif

    void Detach(FrameDetachType type);

    bool IsAttached(void) const { return m_lifecycle.GetState() == FrameLifecycle::kAttached; }

    // IsLoading() is true when the embedder should think a load is in progress.
    // In the case of LocalFrames, it means that the frame has sent a
    // didStartLoading() callback, but not the matching didStopLoading(). Inside
    // blink, you probably want Document::loadEventFinished() instead.
    bool IsLoading(void) const { return m_isLoading; }
    void SetIsLoading(bool isLoading) { m_isLoading = isLoading; }

#ifndef BLINKIT_CRAWLER_ONLY
    TouchAction InheritedEffectiveTouchAction(void) const { return m_inheritedEffectiveTouchAction; }
#endif

    constexpr bool IsLocalFrame(void) const { return true; } // Just a placeholder.
    constexpr bool IsMainFrame(void) const { return true; }  // Just a placeholder.
protected:
#ifdef BLINKIT_CRAWLER_ONLY
    Frame(FrameClient *client);
#else
    Frame(FrameClient *client, Page *page);
#endif

    // DetachImpl() may be re-entered multiple times, if a frame is detached while
    // already being detached.
    virtual void DetachImpl(FrameDetachType type) = 0;

    // Note that IsAttached() and IsDetached() are not strict opposites: frames
    // that are detaching are considered to be in neither state.
    bool IsDetached(void) const { return m_lifecycle.GetState() == FrameLifecycle::kDetached; }

    Member<DOMWindow> m_domWindow;
#ifndef BLINKIT_CRAWLER_ONLY
    Member<Page> m_page;
    TouchAction m_inheritedEffectiveTouchAction = TouchAction::kTouchActionAuto;
#endif
private:
    Member<FrameClient> m_client;
    FrameLifecycle m_lifecycle;
    bool m_isLoading = false;
};

} // namespace blink

#endif // BLINKIT_BLINK_FRAME_H
