#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: rendering_session.h
// Description: RenderingSession Class
//      Author: Ziming Li
//     Created: 2021-11-16
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_RENDERING_SESSION_H
#define BLINKIT_BLINKIT_RENDERING_SESSION_H

#include "blinkit/blink/renderer/platform/geometry/IntRect.h"

class WebViewImpl;

namespace BlinKit {

class RenderingSession
{
public:
    void Enter(WebViewImpl &webView);
    void Leave(WebViewImpl &webView);

    void OnShow(void);
    void OnHide(void);

    void OnLoadStarted(void);
    void OnLoadFinished(void);

    bool AnimationScheduled(void) const { return m_animationScheduled; }
    void ScheduleAnimation(void)
    {
        ASSERT(m_scheduledCount > 0 || !Enabled());
        m_animationScheduled = true;
    }

    void InvalidateRect(const blink::IntRect &rect)
    {
        m_rectToInvalidate.unite(rect);
    }

    void Update(WebViewImpl &webView);
private:
    bool Enabled(void) const { return 0 == m_status; }

    unsigned m_status = 0;
    unsigned m_scheduledCount = 0;
    bool m_animationScheduled = false, m_updated = false;
    blink::IntRect m_rectToInvalidate;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_RENDERING_SESSION_H
