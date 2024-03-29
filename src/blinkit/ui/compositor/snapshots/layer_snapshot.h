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

#include "blinkit/ui/compositor/raster/raster_input.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "third_party/skia/include/core/SkCanvas.h"
#ifndef NDEBUG
#   include "third_party/zed/include/zed/file/path.hpp"
#endif

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

    using UpdateCallback = std::function<void(SkCanvas &)>;
    virtual void Update(const IntSize &viewportSize, const LayerContext &context, const UpdateCallback &callback) = 0;

    virtual void BlendToCanvas(SkCanvas &canvas, const IntRect &dirtyRect, const SkPaint *paint) = 0;

#ifndef NDEBUG
    virtual void DumpTo(zed::path::psz_t path) = 0;
#endif
protected:
    LayerSnapshot(void) = default;
};

} // namespace BlinKit

#endif // BLINKIT_LAYER_SNAPSHOT_H
