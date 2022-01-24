#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: display_item.h
// Description: DisplayItem Class
//      Author: Ziming Li
//     Created: 2021-12-22
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_DISPLAY_ITEM_H
#define BLINKIT_DISPLAY_ITEM_H

#include "blinkit/blink/renderer/platform/geometry/int_rect.h"
#include "third_party/skia/include/core/SkCanvas.h"

namespace BlinKit {

class DisplayItem
{
public:
    virtual ~DisplayItem(void) = default;

    virtual void Playback(SkCanvas &canvas) = 0;
protected:
    DisplayItem(const IntRect &visualRect) : m_visualRect(visualRect) {}
private:
    const IntRect m_visualRect;
};

class RestoreCanvasItem final : public DisplayItem
{
public:
    RestoreCanvasItem(const IntRect &visualRect) : DisplayItem(visualRect) {}
private:
    void Playback(SkCanvas &canvas) override {
        canvas.restore();
    }
};

} // namespace BlinKit

#endif // BLINKIT_DISPLAY_ITEM_H
