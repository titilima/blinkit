// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: posix_thread.h
// Description: PosixThread Class
//      Author: Ziming Li
//     Created: 2020-04-19
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_POSIX_THREAD_H
#define BLINKIT_BLINKIT_POSIX_THREAD_H

#pragma once

#include "blinkit/blink_impl/thread_impl.h"

namespace BlinKit {
    
class PosixThread final : public ThreadImpl
{
private:
    // Thread overrides
    std::shared_ptr<base::SingleThreadTaskRunner> GetTaskRunner(void) const override;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_POSIX_THREAD_H
