#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: raster_task.h
// Description: RasterTask Class
//      Author: Ziming Li
//     Created: 2021-12-24
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_RASTER_TASK_H
#define BLINKIT_RASTER_TASK_H

#include "blinkit/ui/compositor/tasks/compositor_task.h"
#include "blinkit/ui/compositor/raster/raster_input.h"
#include "blinkit/ui/compositor/raster/raster_result.h"

namespace blink {
class WebTaskRunner;
}

namespace BlinKit {

class AnimationProxy;
class Layer;
struct LayerContext;

class RasterTask final : public CompositorTask
{
public:
    RasterTask(const IntSize &viewportSize, AnimationProxy &proxy);

    IntRect GetViewportRect(void) const { return IntRect(IntPoint(), m_viewportSize); }

    const RasterResult& Result(void) const { return m_result; }

    LayerContext& RequireLayerContext(const Layer &layer, const IntSize &layerBounds);
    void Rasterize(const Layer &layer, const IntRect &visibleRect);
    void UpdateDirtyRect(const IntRect &dirtyRect) {
        m_dirtyRect.unite(dirtyRect);
    }

    bool NeedsRebuild(void) const { return m_needsRebuild; }
    bool SetNeedsRebuild(bool needsRebuild)
    {
        bool ret = m_needsRebuild;
        m_needsRebuild = needsRebuild;
        return ret;
    }
private:
    void Run(Compositor &compositor) override;

    const IntSize m_viewportSize;
    AnimationProxy *m_proxy;

    RasterInput m_input;
    RasterResult m_result;
    IntRect m_dirtyRect; // of viewport

    bool m_needsRebuild = false;
};

} // namespace BlinKit

#endif // BLINKIT_RASTER_TASK_H
