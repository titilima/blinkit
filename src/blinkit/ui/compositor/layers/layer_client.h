#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: layer_client.h
// Description: LayerClient Class
//      Author: Ziming Li
//     Created: 2021-12-11
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_LAYER_CLIENT_H
#define BLINKIT_LAYER_CLIENT_H

namespace blink {
class WebDisplayItemList;
}
namespace BlinKit {

class LayerClient
{
public:
    virtual void PaintContents(WebDisplayItemList *displayItemList) = 0;
protected:
    LayerClient(void) = default;
};

} // namespace BlinKit

#endif // BLINKIT_LAYER_CLIENT_H
