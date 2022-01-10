#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: scrollbar_painters.h
// Description: Scrollbar Painter Classes
//      Author: Ziming Li
//     Created: 2022-01-06
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_SCROLLBAR_PAINTERS_H
#define BLINKIT_SCROLLBAR_PAINTERS_H

#include "blinkit/blink/public/platform/web_scrollbar_theme_painter.h"

namespace blink {
class IntPoint;
class IntSize;
class WebScrollbarThemeGeometry;
}

namespace BlinKit {

class ScrollbarPainter
{
public:
    virtual ~ScrollbarPainter(void) = default;

    virtual void Paint(SkCanvas *canvas, const IntSize &bounds) = 0;
};

class ThemedScrollbarPainter final : public ScrollbarPainter
{
public:
    ThemedScrollbarPainter(std::unique_ptr<WebScrollbar> &&scrollbar, WebScrollbarThemePainter painter,
        std::unique_ptr<WebScrollbarThemeGeometry> &geometry);
    ~ThemedScrollbarPainter(void) override;
private:
    void PaintButtons(SkCanvas *canvas);

    void Paint(SkCanvas *canvas, const IntSize &bounds) override;

    class CanvasWrapper;
    std::unique_ptr<WebScrollbar> m_scrollbar;
    WebScrollbarThemePainter m_painter;
    std::unique_ptr<WebScrollbarThemeGeometry> m_geometry;
};

} // namespace BlinKit

#endif // BLINKIT_SCROLLBAR_PAINTERS_H
