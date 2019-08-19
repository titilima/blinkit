// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: apple_thread.h
// Description: AppleThread Class
//      Author: Ziming Li
//     Created: 2019-08-18
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_APPLE_THREAD_H
#define BLINKIT_BLINKIT_APPLE_THREAD_H

#pragma once

#include <pthread.h>
#include "base/mac/scoped_nsobject.h"
#include "thread_impl.h"

@class NSThread;

namespace BlinKit {
    
class AppleThread final : public ThreadImpl
{
public:
    AppleThread(void);
    ~AppleThread(void);

    struct CreateData;
    void Initialize(CreateData &cd);
private:
    base::scoped_nsobject<NSThread> m_nativeThread;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_APPLE_THREAD_H
