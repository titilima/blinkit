#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: layer_snapshot.h
// Description: LayerSnapshot Class
//      Author: Ziming Li
//     Created: 2022-01-11
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_LAYER_SNAPSHOT_H
#define BLINKIT_LAYER_SNAPSHOT_H

#include "blinkit/blink/renderer/platform/geometry/IntRect.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "third_party/skia/include/core/SkCanvas.h"

namespace BlinKit {

class LayerSnapshot
{
public:
    virtual ~LayerSnapshot(void) = default;

    enum class Type {
        Simple, VerticalScrolling, HorizontalScrolling, Gridded
    };

    static LayerSnapshot* Create(Type type, const IntSize &layerBounds, const IntSize &viewportSize);

    static Type AssumeSnapshotType(const IntSize &layerBounds, const IntSize &viewportSize);

    virtual bool TryToReuse(Type assumedType, const IntSize &layerBounds, const IntSize &viewportSize) = 0;

    virtual void Update(const std::function<void(SkCanvas &)> &callback) = 0;

    virtual void BlendToCanvas(SkCanvas &canvas, const IntPoint &from, const IntPoint &to, const IntSize &size) = 0;
protected:
    LayerSnapshot(const IntSize &layerBounds);

    static constexpr unsigned BlockUnit = 256;
    IntSize m_layerBounds;
};

} // namespace BlinKit

#endif // BLINKIT_LAYER_SNAPSHOT_H
