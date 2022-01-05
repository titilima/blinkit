#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: layer_tasks.h
// Description: Layer Task Classes
//      Author: Ziming Li
//     Created: 2021-12-18
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_LAYER_TASKS_H
#define BLINKIT_LAYER_TASKS_H

#include "blinkit/ui/compositor/tasks/compositor_task.h"

namespace BlinKit {

class Layer;

class ReleaseBitmapTask final : public CompositorTask
{
public:
    ReleaseBitmapTask(Layer *layer);
private:
    void Run(Compositor &compositor) override;

    const int m_layerId;
};

} // namespace BlinKit

#endif // BLINKIT_LAYER_TASKS_H
