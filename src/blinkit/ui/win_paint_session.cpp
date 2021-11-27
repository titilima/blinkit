// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: win_paint_session.cpp
// Description: WinPaintSession Classes
//      Author: Ziming Li
//     Created: 2021-11-23
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./win_paint_session.h"

#include "blinkit/ui/web_view_impl.h"
#include "blinkit/ui/win_web_view_host.h"
#include "chromium/base/time/time.h"

using namespace blink;

namespace BlinKit {

void WinPaintSession::Enter(WebViewImpl &view, bool beginFrame)
{
    ++m_enteredTimes;
    if (1 == m_enteredTimes)
    {
        if (beginFrame)
            view.BeginFrame();
    }
}

void WinPaintSession::Leave(WinWebViewHost &host)
{
    --m_enteredTimes;
    if (m_enteredTimes > 0)
        return;

    if (m_animationScheduled)
    {
        if (!m_updated)
            host.GetView()->UpdateAndPaint();
        if (m_damagedRect.isEmpty())
            host.Invalidate();
        else
            host.Invalidate(&m_damagedRect);
    }

    m_animationScheduled = m_updated = false;
    m_damagedRect = IntRect();

    host.ScheduleAnimationTaskIfNecessary();
    m_lastLeftTime = base::Time::Now().ToDoubleT();
}

double WinPaintSession::TimeDeltaSinceLastLeft(void) const
{
    return base::Time::Now().ToDoubleT() - m_lastLeftTime;
}

void WinPaintSession::Update(WebViewImpl &view)
{
    ASSERT(!m_updated);
    view.UpdateAndPaint();
    m_updated = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ScopedPaintSession::ScopedPaintSession(WinWebViewHost &host, WinPaintSession &paintSession, bool beginFrame)
    : m_host(host), m_paintSession(paintSession)
{
    m_paintSession.Enter(*m_host.GetView(), beginFrame);
}

ScopedPaintSession::~ScopedPaintSession(void)
{
    m_paintSession.Leave(m_host);
}

} // namespace BlinKit
