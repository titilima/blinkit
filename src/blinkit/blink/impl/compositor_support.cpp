// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: compositor_support.cpp
// Description: CompositorSupport Class
//      Author: Ziming Li
//     Created: 2021-12-04
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./compositor_support.h"

#include "blinkit/cc/blink/content_layer.h"

using namespace blink;

namespace BlinKit {

WebContentLayer* CompositorSupport::createContentLayer(WebContentLayerClient *client)
{
    return new ContentLayer(client);
}

} // namespace BlinKit
