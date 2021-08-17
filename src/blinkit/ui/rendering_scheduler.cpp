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

#include "./rendering_scheduler.h"

#include "blinkit/ui/web_view_impl.h"
#include "blinkit/blink/renderer/wtf/MainThread.h"

using namespace blink;

namespace BlinKit {

std::unordered_map<WebViewImpl *, RenderingScheduler *> RenderingScheduler::m_allSchedulers;

static RenderingScheduler *g_reservedScheduler = nullptr;

RenderingScheduler::~RenderingScheduler(void)
{
    ASSERT(isMainThread());

    if (m_animationScheduled)
        m_view->UpdateAndPaint();
    if (!m_rectToInvalidate.isEmpty())
        m_view->InvalidateNativeView(m_rectToInvalidate);

    m_allSchedulers.erase(m_view);
}

RenderingScheduler* RenderingScheduler::From(WebViewImpl *view)
{
    ASSERT(isMainThread());
    ASSERT(nullptr != view);

    RenderingScheduler *ret = nullptr;
    if (nullptr != g_reservedScheduler)
    {
        ASSERT(nullptr == g_reservedScheduler->m_view);

        ret = g_reservedScheduler;
        ret->m_view = view;
        m_allSchedulers[view] = ret;

        g_reservedScheduler = nullptr;
    }
    else
    {
        auto it = m_allSchedulers.find(view);
        ASSERT(std::end(m_allSchedulers) != it);
        if (std::end(m_allSchedulers) != it)
            ret = it->second;
    }
    return ret;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

RefPtr<RenderingScheduler> ScopedRenderingScheduler::GetRenderingScheduler(WebViewImpl *view)
{
    ASSERT(isMainThread());

    RenderingScheduler *ret;
    if (nullptr == view)
    {
        ASSERT(nullptr == g_reservedScheduler);
        ret = g_reservedScheduler = new RenderingScheduler(nullptr);
    }
    else
    {
        auto &allSchedulers = RenderingScheduler::m_allSchedulers;

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
    }
    return adoptRef(ret);
}

} // namespace BlinKit
