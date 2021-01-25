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

#include <queue>
#include "bkcommon/bk_mutex.hpp"
#include "bkcommon/bk_signal.hpp"

namespace BlinKit {

class LoaderTask;

struct LoaderSignalTraits {
    static bool Wait(const std::queue<LoaderTask *> &q) { return !q.empty(); }
};

class LoaderThread
{
public:
    static std::unique_ptr<LoaderThread> Create(void);
    virtual ~LoaderThread(void) = default;

    void AddTask(LoaderTask *task);
protected:
    LoaderThread(void) = default;

    void Exit(void) { AddTask(nullptr); }
    void Run(void);
private:
    BkMutex m_mutex;
    BkSignal m_signal;
    std::queue<LoaderTask *> m_tasks;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_LOADER_THREAD_H
