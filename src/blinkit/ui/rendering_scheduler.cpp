// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: rendering_scheduler.cpp
// Description: RenderingScheduler Classes
//      Author: Ziming Li
//     Created: 2021-01-20
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "rendering_scheduler.h"

#include "blinkit/ui/web_view_impl.h"
#include "third_party/blink/renderer/platform/wtf/wtf.h"

using namespace blink;

namespace BlinKit {

std::unordered_map<WebViewImpl *, RenderingScheduler *> RenderingScheduler::m_allSchedulers;

RenderingScheduler::~RenderingScheduler(void)
{
    ASSERT(IsMainThread());

    if (m_animationScheduled)
        m_view->UpdateAndPaint();
    if (!m_rectToInvalidate.IsEmpty())
        m_view->InvalidateNativeView(m_rectToInvalidate);

    m_allSchedulers.erase(m_view);
}

RenderingScheduler* RenderingScheduler::From(WebViewImpl *view)
{
    ASSERT(IsMainThread());
    ASSERT(nullptr != view);

    auto it = m_allSchedulers.find(view);
    if (std::end(m_allSchedulers) != it)
        return it->second;

    ASSERT(std::end(m_allSchedulers) != it);
    static RenderingScheduler s_nullScheduler(nullptr);
    return &s_nullScheduler;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

RenderingScheduler* ScopedRenderingScheduler::GetRenderingScheduler(WebViewImpl *view)
{
    ASSERT(IsMainThread());
    ASSERT(nullptr != view);

    auto &allSchedulers = RenderingScheduler::m_allSchedulers;

    RenderingScheduler *ret;
    auto it = allSchedulers.find(view);
    if (std::end(allSchedulers) == it)
    {
        ret = new RenderingScheduler(view);
        allSchedulers[view] = ret;
    }
    else
    {
        ret = it->second;
    }
    return ret;
}

} // namespace BlinKit
