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

#include "blinkit/app/caller.h"
#include "blinkit/ui/web_view_impl.h"
#include "blinkit/blink/renderer/wtf/MainThread.h"

using namespace blink;

namespace BlinKit {

static std::unordered_map<WebViewImpl *, RenderingScheduler *> g_allSchedulers;

RenderingScheduler::RenderingScheduler(WebViewImpl &view) : m_view(view)
{
    const auto callback = [this] {
        if (zed::key_exists(g_allSchedulers, &m_view))
            return;

        m_enabled = true;
        g_allSchedulers.emplace(&m_view, this);

        m_view.BeginFrame();
    };
    m_view.GetCaller().SyncCall(BLINK_FROM_HERE, callback);
}

RenderingScheduler::~RenderingScheduler(void)
{
    if (!m_enabled)
        return;

    const auto callback = [this] {
        if (m_animationScheduled)
        {
            if (!m_updated)
                m_view.UpdateAndPaint();
            if (m_rectToInvalidate.isEmpty())
                m_view.InvalidateNativeView();
            else
                m_view.InvalidateNativeView(&m_rectToInvalidate);
        }
        else
        {
            m_view.PostAnimationTask();
        }
        g_allSchedulers.erase(&m_view);
    };
    m_view.GetCaller().SyncCall(BLINK_FROM_HERE, callback);
}

RenderingScheduler* RenderingScheduler::From(WebViewImpl &view)
{
    ASSERT(isMainThread());
    return zed::query_value(g_allSchedulers, &view);
}

void RenderingScheduler::Update(void)
{
    ASSERT(isMainThread());
    ASSERT(!m_updated);
    m_view.UpdateAndPaint();
    m_updated = true;
}

} // namespace BlinKit
