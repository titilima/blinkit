// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: rendering_session.cpp
// Description: RenderingSession Class
//      Author: Ziming Li
//     Created: 2021-11-16
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./rendering_session.h"

#include "blinkit/blink/renderer/wtf/MainThread.h"
#include "blinkit/ui/web_view_impl.h"

using namespace blink;

namespace BlinKit {

class RenderingStatus {
public:
    enum {
        Hide    = 0x1,
        Loading = 0x2
    };
};

void RenderingSession::Enter(WebViewImpl &webView)
{
    ASSERT(isMainThread());
    if (1 == ++m_scheduledCount)
        webView.BeginFrame();
}

void RenderingSession::Leave(WebViewImpl &webView)
{
    ASSERT(isMainThread());
    ASSERT(m_scheduledCount > 0);
    if (--m_scheduledCount > 0)
        return;

    if (Enabled())
    {
        if (m_animationScheduled)
        {
            if (!m_updated)
                webView.UpdateAndPaint();
            if (m_rectToInvalidate.isEmpty())
                webView.InvalidateNativeView();
            else
                webView.InvalidateNativeView(&m_rectToInvalidate);
        }
        else
        {
            webView.PostAnimationTask();
        }
    }

    m_animationScheduled = m_updated = false;
    m_rectToInvalidate = IntRect();
}

void RenderingSession::OnHide(void)
{
    m_status |= RenderingStatus::Hide;
}

void RenderingSession::OnLoadFinished(void)
{
    m_status &= ~RenderingStatus::Loading;
}

void RenderingSession::OnLoadStarted(void)
{
    m_status |= RenderingStatus::Loading;
}

void RenderingSession::OnShow(void)
{
    m_status &= ~RenderingStatus::Hide;
}

void RenderingSession::Update(WebViewImpl &webView)
{
    webView.UpdateAndPaint();
    m_updated = true;
}

} // namespace BlinKit
