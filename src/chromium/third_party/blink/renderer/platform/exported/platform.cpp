// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: platform.cpp
// Description: Platform Class
//      Author: Ziming Li
//     Created: 2019-09-11
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "public/platform/platform.h"

namespace blink {

static Platform *g_platform = nullptr;

Platform* Platform::Current(void)
{
    return g_platform;
}

void Platform::Initialize(Platform *platform, scheduler::WebThreadScheduler *mainThreadScheduler)
{
    DCHECK(!g_platform);
    DCHECK(platform);
    g_platform = platform;
#if 0 // BKTODO:
    g_platform->owned_main_thread_ = main_thread_scheduler->CreateMainThread();
    g_platform->main_thread_ = g_platform->owned_main_thread_.get();
    DCHECK(!g_platform->current_thread_slot_.Get());
    g_platform->current_thread_slot_.Set(g_platform->main_thread_);
#endif // 0
    InitializeCommon(platform);
}

void Platform::InitializeCommon(Platform *platform)
{
    assert(false); // BKTODO:
}

} // namespace blink
