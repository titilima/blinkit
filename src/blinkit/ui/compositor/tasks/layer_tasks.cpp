// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: layer_tasks.cpp
// Description: Layer Task Classes
//      Author: Ziming Li
//     Created: 2021-12-18
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./layer_tasks.h"

#include "blinkit/ui/compositor/blink/layer.h"
#include "blinkit/ui/compositor/compositor.h"

namespace BlinKit {

ReleaseBitmapTask::ReleaseBitmapTask(Layer *layer) : m_layerId(layer->id())
{
}

void ReleaseBitmapTask::Run(Compositor &compositor)
{
    compositor.ReleaseBitmap(m_layerId);
}

} // namespace BlinKit
