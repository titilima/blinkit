// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: content_layer.cpp
// Description: ContentLayer Class
//      Author: Ziming Li
//     Created: 2021-12-04
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./content_layer.h"

#include "blinkit/ui/compositor/blink/layer.h"

using namespace blink;

namespace BlinKit {

ContentLayer::ContentLayer(WebContentLayerClient *client) : m_client(client), m_layer(std::make_unique<Layer>())
{
}

ContentLayer::~ContentLayer(void) = default;

WebLayer* ContentLayer::layer(void)
{
    return m_layer.get();
}

} // namespace BlinKit
