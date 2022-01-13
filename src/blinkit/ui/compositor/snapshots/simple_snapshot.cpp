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

SimpleSnapshot::SimpleSnapshot(const IntSize &layerBounds) : LayerSnapshot(layerBounds)
{
    Resize();
}

void SimpleSnapshot::BlendToCanvas(SkCanvas &canvas, const IntPoint &from, const IntPoint &to, const IntSize &size)
{
    canvas.drawBitmapRect(
        m_bitmap,
        SkIRect::MakeXYWH(from.x(), from.y(), size.width(), size.height()),
        SkRect::MakeXYWH(to.x(), to.y(), size.width(), size.height()),
        nullptr
    );
}

void SimpleSnapshot::Resize(void)
{
    m_bitmap.allocN32Pixels(m_layerBounds.width(), m_layerBounds.height(), kPremul_SkAlphaType);
}

bool SimpleSnapshot::TryToReuse(Type assumedType, const IntSize &layerBounds, const IntSize &)
{
    const SkImageInfo &info = m_bitmap.info();

    bool sizeIsFit = (info.width() >= layerBounds.width()) && (info.height() >= layerBounds.height());

    if (Type::Simple != assumedType)
    {
        if (!sizeIsFit)
            return false;
    }

    m_layerBounds = layerBounds;
    if (!sizeIsFit)
        Resize();
    return true;
}

void SimpleSnapshot::Update(const std::function<void(SkCanvas &)> &callback)
{
    SkCanvas canvas(m_bitmap);
    callback(canvas);
}

} // namespace BlinKit
