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
#include "blinkit/blink/renderer/wtf/Allocator.h"

class WebViewImpl;

namespace blink {
class Document;
}

namespace BlinKit {

class RenderingScheduler final
{
    STACK_ALLOCATED();
public:
    RenderingScheduler(WebViewImpl &view);
    RenderingScheduler(blink::Document &document);
    ~RenderingScheduler(void);

    static RenderingScheduler* From(WebViewImpl &view);

    void ScheduleAnimation(void) { m_animationScheduled = true; }
    bool AnimationScheduled(void) const { return m_animationScheduled; }

    void InvalidateRect(const blink::IntRect &rect) { m_rectToInvalidate.unite(rect); }
    void Update(void);
private:
    WebViewImpl &m_view;
    double m_tick = 0.0;
    bool m_enabled = false;
    bool m_updated = false;
    bool m_animationScheduled = false;
    blink::IntRect m_rectToInvalidate;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_RENDERING_SCHEDULER_H
