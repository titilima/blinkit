#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: compositor_support.h
// Description: CompositorSupport Class
//      Author: Ziming Li
//     Created: 2021-12-04
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_COMPOSITOR_SUPPORT_H
#define BLINKIT_COMPOSITOR_SUPPORT_H

#include "blinkit/blink/public/platform/WebCompositorSupport.h"

namespace BlinKit {

class CompositorSupport final : public blink::WebCompositorSupport
{
public:
    blink::WebContentLayer* createContentLayer(blink::WebContentLayerClient *client) override;
};

} // namespace BlinKit

#endif // BLINKIT_COMPOSITOR_SUPPORT_H
