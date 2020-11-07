// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: loader_thread.cpp
// Description: LoaderThread Class
//      Author: Ziming Li
//     Created: 2020-11-04
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "loader_thread.h"

#include "blinkit/loader/loader_task.h"
#include "third_party/blink/renderer/platform/wtf/wtf.h"

namespace BlinKit {

void LoaderThread::AddTask(LoaderTask *task)
{
    ASSERT(IsMainThread());
    Access([task](std::queue<LoaderTask *> &q) {
        q.push(task);
    });
    Notify();
}

void LoaderThread::Run(void)
{
    ASSERT(!IsMainThread());

    for (;;)
    {
        LoaderTask *task = nullptr;
        Wait([&task](std::queue<LoaderTask *> &q) {
            task = q.front();
            q.pop();
        });

        if (nullptr == task)
            break;

        task->Run();
    }
}

} // namespace BlinKit
