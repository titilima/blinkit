#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: content_layer.h
// Description: ContentLayer Class
//      Author: Ziming Li
//     Created: 2021-12-04
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_CONTENT_LAYER_H
#define BLINKIT_CONTENT_LAYER_H

#include "blinkit/blink/public/platform/web_content_layer.h"

namespace blink {
class WebContentLayerClient;
}

namespace BlinKit {

class Layer;

class ContentLayer final : public blink::WebContentLayer
{
public:
    ContentLayer(blink::WebContentLayerClient *client);
    ~ContentLayer(void) override;
private:
    blink::WebLayer* layer(void) override;

    blink::WebContentLayerClient *m_client;
    std::unique_ptr<Layer> m_layer;
};

} // namespace BlinKit

#endif // BLINKIT_CONTENT_LAYER_H
