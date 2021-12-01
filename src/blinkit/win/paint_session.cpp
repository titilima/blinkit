// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: paint_session.cpp
// Description: PaintSession Class
//      Author: Ziming Li
//     Created: 2021-11-23
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./paint_session.h"

#include "blinkit/ui/web_view_impl.h"
#include "blinkit/ui/win_web_view_host.h"
#include "blinkit/win/scoped_paint_scheduler.h"

using namespace blink;

namespace BlinKit {

PaintSession::~PaintSession(void)
{
    if (m_attached)
        ScopedPaintScheduler::Current()->Detach(*this);
}

void PaintSession::AttachToScheduler(WinWebViewHost &host)
{
    ASSERT(!m_attached);
    ScopedPaintScheduler::Current()->Attach(*this, host);
    ASSERT(m_attached);
}

void PaintSession::Begin(WinWebViewHost &host)
{
    if (!m_beginFrameCalled)
    {
        double tick = monotonicallyIncreasingTime();
        host.GetView()->BeginFrame(tick);
        m_beginFrameCalled = true;
    }
}

void PaintSession::DetachFromScheduler(void)
{
    ASSERT(m_attached);
    ScopedPaintScheduler::Current()->Detach(*this);
    ASSERT(!m_attached);
}

void PaintSession::Flush(WinWebViewHost &host)
{
    HWND hWnd = host.GetHWND();

    host.GetView()->UpdateLifecycle();

    RECT rc = { 0 };
    if (m_damagedRect.isEmpty())
    {
        const IntSize &size = host.GetView()->Size();
        m_damagedRect.setSize(size);

        rc.right  = size.width();
        rc.bottom = size.height();
    }
    else
    {
        rc = { m_damagedRect.x(), m_damagedRect.y(), m_damagedRect.maxX(), m_damagedRect.maxY() };
    }
    host.Redraw(m_damagedRect);
    m_damagedRect = IntRect();
    InvalidateRect(hWnd, &rc, FALSE);

    m_beginFrameCalled = false;
}

} // namespace BlinKit
