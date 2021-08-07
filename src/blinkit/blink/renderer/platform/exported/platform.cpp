// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: platform.cpp
// Description: Platform Class
//      Author: Ziming Li
//     Created: 2019-09-11
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "blinkit/blink/public/platform/Platform.h"

namespace blink {

static Platform *g_platform = nullptr;

Platform::Platform(void) : m_mainThread(nullptr)
{
}

Platform* Platform::current(void)
{
    return g_platform;
}

void Platform::initialize(Platform *platform)
{
    ASSERT(nullptr == g_platform);
    ASSERT(nullptr != platform);
    g_platform = platform;
}

} // namespace blink
