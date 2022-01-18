#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: vertical_scrolling_snapshot.h
// Description: VerticalScrollingSnapshot Class
//      Author: Ziming Li
//     Created: 2022-01-13
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_VERTICAL_SCROLLING_SNAPSHOT_H
#define BLINKIT_VERTICAL_SCROLLING_SNAPSHOT_H

#include "blinkit/ui/compositor/snapshots/single_axis_scrolling_snapshot.h"

namespace BlinKit {

class VerticalScrollingSnapshot final : public SingleAxisScrollingSnapshot
{
public:
    VerticalScrollingSnapshot(void) = default;
private:
    bool TryToReuse(Type assumedType, const IntSize &layerBounds, const IntSize &viewportSize) override;
    Metrics GetMetrics(const IntSize &layerBounds, const IntSize &viewportSize) const override;
};

} // namespace BlinKit

#endif // BLINKIT_VERTICAL_SCROLLING_SNAPSHOT_H
