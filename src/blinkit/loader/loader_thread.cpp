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

#include "./loader_thread.h"

#include "blinkit/blink/impl/thread.h"
#include "blinkit/blink/renderer/wtf/MainThread.h"
#include "blinkit/loader/loader_task.h"

using namespace blink;

namespace BlinKit {

void LoaderThread::AddTask(LoaderTask *task)
{
    ASSERT(isMainThread());
    zed::task_thread::add(task);
}

#ifndef NDEBUG
void LoaderThread::on_enter_loop(void)
{
    zed::current_thread::set_name("Loader Thread");
}
#endif

} // namespace BlinKit
