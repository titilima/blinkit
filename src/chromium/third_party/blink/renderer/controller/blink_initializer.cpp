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

#include "public/web/blink.h"

namespace blink {

static void InitializeCommon(Platform *platform)
{
    assert(false); // BKTODO:
}

void Initialize(Platform *platform, scheduler::WebThreadScheduler* mainThreadScheduler)
{
    Platform::Initialize(platform, mainThreadScheduler);
    InitializeCommon(platform);
}

} // namespace blink
