// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: blink_initializer.cpp
// Description: Blink Initializer
//      Author: Ziming Li
//     Created: 2019-09-11
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "blinkit/blink/public/web/blink.h"

namespace blink {

static void InitializeCommon(Platform *platform)
{
}

void Initialize(Platform *platform)
{
    Platform::initialize(platform);
    InitializeCommon(platform);
}

} // namespace blink
