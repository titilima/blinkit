// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: web_widget.h
// Description: Placeholders for WebWidget Stuff
//      Author: Ziming Li
//     Created: 2020-12-15
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_WEB_WIDGET_H
#define BLINKIT_BLINK_WEB_WIDGET_H

#pragma once

#include "cc/paint/paint_canvas.h"
#include "third_party/blink/public/platform/web_input_event_result.h"

namespace blink {

class WebWidget
{
public:
    enum class LifecycleUpdate { kLayout, kPrePaint, kAll };
protected:
    WebWidget(void) = default;
};

} // namespace blink

#endif // BLINKIT_BLINK_WEB_WIDGET_H
