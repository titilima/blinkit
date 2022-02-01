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

void SimpleSnapshot::BlendToCanvas(SkCanvas &canvas, const IntRect &dirtyRect)
{
    IntRect rect = intersection(m_tile.Rect(), dirtyRect);
    if (!rect.isEmpty())
        m_tile.BlendToCanvas(canvas, rect);
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
        ASSERT(false); // BKTODO: Resize(m_bitmap, m_layerBounds);
    return true;
}

void SimpleSnapshot::Update(const IntSize &viewportSize, const LayerContext &context, const UpdateCallback &callback)
{
    m_tile.UpdatePosition(context.positionOfViewport);

    IntRect dirtyRect(context.dirtyRectOfLayer);
    dirtyRect.moveBy(context.positionOfViewport);
    m_tile.Update(dirtyRect, callback);
}

} // namespace BlinKit
