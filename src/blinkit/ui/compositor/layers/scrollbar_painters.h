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
#include "blinkit/blink/renderer/platform/geometry/int_rect.h"

class SkPicture;

namespace blink {
class WebScrollbarThemeGeometry;
}

namespace BlinKit {

class ScrollbarPainter
{
public:
    virtual ~ScrollbarPainter(void) = default;

    virtual SkPicture* Paint(const IntSize &bounds) = 0;
};

class ThemedScrollbarPainter final : public ScrollbarPainter
{
public:
    ThemedScrollbarPainter(std::unique_ptr<WebScrollbar> &&scrollbar, WebScrollbarThemePainter painter,
        std::unique_ptr<WebScrollbarThemeGeometry> &geometry);
    ~ThemedScrollbarPainter(void) override;
private:
    void PaintButtons(SkCanvas *canvas);
    void PaintTrack(SkCanvas *canvas);

    SkPicture* Paint(const IntSize &bounds) override;

    std::unique_ptr<WebScrollbar> m_scrollbar;
    WebScrollbarThemePainter m_painter;
    std::unique_ptr<WebScrollbarThemeGeometry> m_geometry;
};

} // namespace BlinKit

#endif // BLINKIT_SCROLLBAR_PAINTERS_H
