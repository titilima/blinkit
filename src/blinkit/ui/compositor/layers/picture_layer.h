#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: picture_layer.h
// Description: PictureLayer Class
//      Author: Ziming Li
//     Created: 2021-12-11
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_PICTURE_LAYER_H
#define BLINKIT_PICTURE_LAYER_H

#include "blinkit/ui/compositor/blink/layer.h"

namespace BlinKit {

class PictureLayer final : public Layer
{
public:
    PictureLayer(LayerClient *client);
};

} // namespace BlinKit

#endif // BLINKIT_PICTURE_LAYER_H
