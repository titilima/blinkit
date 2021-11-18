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

using namespace blink;

namespace BlinKit {

RenderingScheduler::RenderingScheduler(WebViewImpl &view) : m_view(view)
{
    m_view.EnterRenderingSession();
}

RenderingScheduler::RenderingScheduler(Document &document) : RenderingScheduler(*WebViewImpl::From(document))
{
}

RenderingScheduler::~RenderingScheduler(void)
{
    m_view.LeaveRenderingSession();
}

} // namespace BlinKit
