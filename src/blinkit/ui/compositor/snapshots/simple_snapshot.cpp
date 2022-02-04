// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: simple_snapshot.cpp
// Description: SimpleSnapshot Class
//      Author: Ziming Li
//     Created: 2022-01-12
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#include "./simple_snapshot.h"

namespace BlinKit {

SimpleSnapshot::SimpleSnapshot(const IntSize &layerBounds) : m_tile(layerBounds)
{
}

void SimpleSnapshot::BlendToCanvas(SkCanvas &canvas, const IntRect &dirtyRect, const SkPaint *paint)
{
    IntRect rect = intersection(m_tile.Rect(), dirtyRect);
    if (!rect.isEmpty())
        m_tile.BlendToCanvas(canvas, rect, paint);
}

bool SimpleSnapshot::TryToReuse(Type assumedType, const IntSize &layerBounds, const IntSize &)
{
    const SkImageInfo &info = m_tile.Bitmap().info();

    bool sizeIsFit = (info.width() >= layerBounds.width()) && (info.height() >= layerBounds.height());

    if (Type::Simple != assumedType)
    {
        if (!sizeIsFit)
            return false;
    }

    if (!sizeIsFit)
    {
        IntRect rect = m_tile.Rect();
        rect.setSize(layerBounds);
        m_tile.Reset(rect);
    }
    return true;
}

void SimpleSnapshot::Update(const IntSize &viewportSize, const LayerContext &context, const UpdateCallback &callback)
{
    m_tile.UpdatePosition(context.positionOfViewport);

    IntRect dirtyRect;
    if (m_tile.Painted())
    {
        dirtyRect = context.dirtyRectOfLayer;
        dirtyRect.moveBy(context.positionOfViewport);
        dirtyRect.intersect(m_tile.Rect());
        if (dirtyRect.isEmpty())
            return;
    }
    else
    {
        dirtyRect.setSize(m_tile.Rect().size());
    }

    m_tile.Update(dirtyRect, callback);
}

} // namespace BlinKit
