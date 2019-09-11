// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: platform.h
// Description: Platform Class
//      Author: Ziming Li
//     Created: 2019-09-11
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_PLATFORM_H
#define BLINKIT_BLINK_PLATFORM_H

#pragma once

namespace blink {

namespace scheduler {
class WebThreadScheduler;
}

class Platform
{
public:
    static void Initialize(Platform *platform, scheduler::WebThreadScheduler *mainThreadScheduler);
    static Platform* Current(void);
private:
    static void InitializeCommon(Platform *platform);
};

} // namespace blink

#endif // BLINKIT_BLINK_PLATFORM_H
