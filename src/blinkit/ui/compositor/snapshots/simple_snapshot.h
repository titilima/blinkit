#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: simple_snapshot.h
// Description: SimpleSnapshot Class
//      Author: Ziming Li
//     Created: 2022-01-12
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_SIMPLE_SNAPSHOT_H
#define BLINKIT_SIMPLE_SNAPSHOT_H

#include "blinkit/ui/compositor/snapshots/layer_snapshot.h"

namespace BlinKit {

class SimpleSnapshot final : public LayerSnapshot
{
public:
    SimpleSnapshot(const IntSize &layerBounds);
private:
    void Resize(void);

    bool TryToReuse(Type assumedType, const IntSize &layerBounds, const IntSize &viewportSize) override;
    void Update(const std::function<void(SkCanvas &)> &callback) override;
    void BlendToCanvas(SkCanvas &canvas, const IntPoint &from, const IntPoint &to, const IntSize &size) override;

    SkBitmap m_bitmap;
};

} // namespace BlinKit

#endif // BLINKIT_SIMPLE_SNAPSHOT_H
