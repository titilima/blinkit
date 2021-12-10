// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: loader_thread.h
// Description: LoaderThread Class
//      Author: Ziming Li
//     Created: 2020-11-04
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_LOADER_THREAD_H
#define BLINKIT_BLINKIT_LOADER_THREAD_H

#pragma once

#include "third_party/zed/include/zed/threading/task_queue.hpp"

namespace BlinKit {

class LoaderTask;

class LoaderThread final : private zed::task_thread
{
public:
    LoaderThread(void) = default;

    void AddTask(LoaderTask *task);
private:
#ifndef NDEBUG
    void on_enter_loop(void) override;
#endif
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_LOADER_THREAD_H
