#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: graphics_layer_factory_impl.h
// Description: GraphicsLayerFactoryImpl Class
//      Author: Ziming Li
//     Created: 2021-12-04
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_GRAPHICS_LAYER_FACTORY_IMPL_H
#define BLINKIT_GRAPHICS_LAYER_FACTORY_IMPL_H

#include "blinkit/blink/renderer/platform/graphics/GraphicsLayerFactory.h"

class WebViewImpl;

namespace BlinKit {

class GraphicsLayerFactoryImpl final : public blink::GraphicsLayerFactory
{
public:
    explicit GraphicsLayerFactoryImpl(WebViewImpl &webView);
private:
    std::unique_ptr<blink::GraphicsLayer> createGraphicsLayer(blink::GraphicsLayerClient *client) override;

    WebViewImpl &m_webView;
};

} // namespace BlinKit

#endif // BLINKIT_GRAPHICS_LAYER_FACTORY_IMPL_H
