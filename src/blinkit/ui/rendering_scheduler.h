#pragma once
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
private:
    WebViewImpl &m_view;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_RENDERING_SCHEDULER_H
