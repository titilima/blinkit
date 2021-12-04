// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: graphics_layer_factory_impl.cpp
// Description: GraphicsLayerFactoryImpl Class
//      Author: Ziming Li
//     Created: 2021-12-04
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./graphics_layer_factory_impl.h"

#include "blinkit/blink/renderer/platform/graphics/GraphicsLayer.h"
#include "blinkit/ui/web_view_impl.h"

using namespace blink;

namespace BlinKit {

GraphicsLayerFactoryImpl::GraphicsLayerFactoryImpl(WebViewImpl &webView) : m_webView(webView)
{
}

std::unique_ptr<GraphicsLayer> GraphicsLayerFactoryImpl::createGraphicsLayer(GraphicsLayerClient *client)
{
    std::unique_ptr<GraphicsLayer> layer = zed::wrap_unique(new GraphicsLayer(client));
    m_webView.registerForAnimations(layer->platformLayer());
    return layer;
}

} // namespace BlinKit
