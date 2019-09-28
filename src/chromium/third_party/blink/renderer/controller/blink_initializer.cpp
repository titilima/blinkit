// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: blink_initializer.cpp
// Description: Blink Initializer
//      Author: Ziming Li
//     Created: 2019-09-11
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "third_party/blink/public/web/blink.h"

namespace blink {

static void InitializeCommon(Platform *platform)
{
    // BKTODO:
}

void Initialize(Platform *platform, scheduler::WebThreadScheduler* mainThreadScheduler)
{
    Platform::Initialize(platform, mainThreadScheduler);
    InitializeCommon(platform);
}

} // namespace blink
