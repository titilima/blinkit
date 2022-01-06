// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: scrollbar_painters.cpp
// Description: Scrollbar Painter Classes
//      Author: Ziming Li
//     Created: 2022-01-06
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#include "./scrollbar_painters.h"

#include "blinkit/blink/public/platform/web_scrollbar.h"
#include "blinkit/blink/public/platform/web_scrollbar_theme_geometry.h"

namespace BlinKit {

ThemedScrollbarPainter::ThemedScrollbarPainter(
    std::unique_ptr<WebScrollbar> &&scrollbar,
    WebScrollbarThemePainter painter,
    std::unique_ptr<WebScrollbarThemeGeometry> &geometry)
    : m_scrollbar(std::move(scrollbar))
    , m_painter(painter)
    , m_geometry(std::move(geometry))
{
}

ThemedScrollbarPainter::~ThemedScrollbarPainter(void) = default;

void ThemedScrollbarPainter::Paint(SkCanvas *canvas, const IntSize &bounds)
{
    IntRect rect;

    rect.setSize(bounds);
    m_painter.paintScrollbarBackground(canvas, rect);

    const IntPoint offset = -m_scrollbar->location();
    if (m_geometry->hasButtons(m_scrollbar.get()))
        PaintButtons(canvas, offset);

    rect = m_geometry->trackRect(m_scrollbar.get());
    if (!rect.isEmpty())
    {
        rect.moveBy(offset);
        m_painter.paintTrackBackground(canvas, rect);

        if (m_geometry->hasThumb(m_scrollbar.get()))
        {
            m_painter.paintForwardTrackPart(canvas, rect);
            m_painter.paintBackTrackPart(canvas, rect);
        }

        m_painter.paintTickmarks(canvas, rect);
    }
}

void ThemedScrollbarPainter::PaintButtons(SkCanvas *canvas, const IntPoint &offset)
{
    IntRect rect;

    rect = m_geometry->backButtonStartRect(m_scrollbar.get());
    if (!rect.isEmpty())
    {
        rect.moveBy(offset);
        m_painter.paintBackButtonStart(canvas, rect);
    }

    rect = m_geometry->backButtonEndRect(m_scrollbar.get());
    if (!rect.isEmpty())
    {
        rect.moveBy(offset);
        m_painter.paintBackButtonEnd(canvas, rect);
    }

    rect = m_geometry->forwardButtonStartRect(m_scrollbar.get());
    if (!rect.isEmpty())
    {
        rect.moveBy(offset);
        m_painter.paintForwardButtonStart(canvas, rect);
    }

    rect = m_geometry->forwardButtonEndRect(m_scrollbar.get());
    if (!rect.isEmpty())
    {
        rect.moveBy(offset);
        m_painter.paintForwardButtonEnd(canvas, rect);
    }
}

} // namespace BlinKit
