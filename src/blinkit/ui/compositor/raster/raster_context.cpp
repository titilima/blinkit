// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: raster_context.cpp
// Description: RasterContext Class
//      Author: Ziming Li
//     Created: 2022-01-04
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#include "./raster_context.h"

#include "blinkit/blink/renderer/wtf/MainThread.h"
#include "blinkit/ui/compositor/blink/layer.h"

namespace BlinKit {

RasterContext::RasterContext(void)
{
    ASSERT(isMainThread());
}

RasterContext::RasterContext(const RasterContext &parent, const Layer &currentLayer)
    : m_offset(parent.m_offset + currentLayer.Position())
{
    ASSERT(isMainThread());
}

} // namespace BlinKit
