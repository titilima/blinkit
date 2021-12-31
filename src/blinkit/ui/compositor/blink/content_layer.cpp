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

#include "blinkit/blink/public/platform/WebContentLayerClient.h"
#include "blinkit/ui/compositor/layers/picture_layer.h"

namespace BlinKit {

ContentLayer::ContentLayer(WebContentLayerClient *client)
    : m_client(client), m_layer(std::make_unique<PictureLayer>(this))
{
}

ContentLayer::~ContentLayer(void) = default;

WebLayer* ContentLayer::layer(void)
{
    return m_layer.get();
}

void ContentLayer::PaintContents(WebDisplayItemList *displayItemList)
{
    m_client->paintContents(displayItemList);
}

} // namespace BlinKit
