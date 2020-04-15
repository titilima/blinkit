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

#include "blinkit/blink_impl/thread_impl.h"

namespace BlinKit {
    
class AppleThread final : public ThreadImpl
{
private:
    // Thread overrides
    std::shared_ptr<base::SingleThreadTaskRunner> GetTaskRunner(void) const override;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_APPLE_THREAD_H
