#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: win_paint_session.h
// Description: WinPaintSession Classes
//      Author: Ziming Li
//     Created: 2021-11-23
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_WIN_PAINT_SESSION_H
#define BLINKIT_WIN_PAINT_SESSION_H

#include "blinkit/blink/renderer/platform/geometry/IntRect.h"
#include "blinkit/blink/renderer/wtf/Allocator.h"

class WebViewImpl;

namespace BlinKit {

class WinWebViewHost;

class WinPaintSession
{
public:
    void Enter(WebViewImpl &view, bool beginFrame);
    void Leave(WinWebViewHost &host);

    void Update(WebViewImpl &view);

    bool AnimationScheduled(void) const { return m_animationScheduled; }
    void ScheduleAnimation(void) { m_animationScheduled = true; }
private:
    unsigned m_enteredTimes = 0;

    bool m_animationScheduled = false;
    bool m_updated = false;

    blink::IntRect m_damagedRect;
};

class ScopedPaintSession
{
    STACK_ALLOCATED();
public:
    ScopedPaintSession(WinWebViewHost &host, WinPaintSession &paintSession, bool beginFrame = true);
    ~ScopedPaintSession(void);
private:
    WinWebViewHost &m_host;
    WinPaintSession &m_paintSession;
};

} // namespace BlinKit

#endif // BLINKIT_WIN_PAINT_SESSION_H
