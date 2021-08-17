// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: rendering_scheduler.h
// Description: RenderingScheduler Classes
//      Author: Ziming Li
//     Created: 2021-01-20
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_RENDERING_SCHEDULER_H
#define BLINKIT_BLINKIT_RENDERING_SCHEDULER_H

#pragma once

#include "blinkit/blink/renderer/platform/geometry/IntRect.h"
#include "blinkit/blink/renderer/wtf/RefCounted.h"
#include "blinkit/blink/renderer/wtf/RefPtr.h"

class WebViewImpl;

namespace BlinKit {

class RenderingScheduler final : public RefCounted<RenderingScheduler>
{
public:
    static RenderingScheduler* From(WebViewImpl *view);
    ~RenderingScheduler(void);

    void ScheduleAnimation(void) { m_animationScheduled = true; }
    void InvalidateRect(const blink::IntRect &rect) { m_rectToInvalidate.unite(rect); }
private:
    friend class ScopedRenderingScheduler;

    RenderingScheduler(WebViewImpl *view) : m_view(view) {}

    static std::unordered_map<WebViewImpl *, RenderingScheduler *> m_allSchedulers;

    WebViewImpl *m_view;
    bool m_animationScheduled = false;
    blink::IntRect m_rectToInvalidate;
};

class ScopedRenderingScheduler
{
public:
    ScopedRenderingScheduler(WebViewImpl *view = nullptr) : m_renderingScheduler(GetRenderingScheduler(view)) {}
private:
    static RefPtr<RenderingScheduler> GetRenderingScheduler(WebViewImpl *view);

    RefPtr<RenderingScheduler> m_renderingScheduler;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_RENDERING_SCHEDULER_H
