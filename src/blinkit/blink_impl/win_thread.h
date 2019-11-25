// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: win_thread.h
// Description: WinThread Class
//      Author: Ziming Li
//     Created: 2019-03-12
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_WIN_THREAD_H
#define BLINKIT_BLINKIT_WIN_THREAD_H

#pragma once

#include "blinkit/blink_impl/thread_impl.h"

namespace BlinKit {

class WinThread final : public ThreadImpl
{
public:
    ~WinThread(void) override;
private:
    // Thread overrides
    std::shared_ptr<base::SingleThreadTaskRunner> GetTaskRunner(void) const override;

    HANDLE m_hThread = nullptr;
    mutable std::shared_ptr<base::SingleThreadTaskRunner> m_taskRunner;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_WIN_THREAD_H
