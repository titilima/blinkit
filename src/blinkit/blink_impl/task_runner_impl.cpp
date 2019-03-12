// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: task_runner_impl.cpp
// Description: TaskRunnerImpl Class
//      Author: Ziming Li
//     Created: 2019-03-12
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "task_runner_impl.h"

using namespace blink;

namespace BlinKit {

TaskRunnerImpl::TaskRunnerImpl(ThreadImpl &thread) : m_thread(thread)
{
    // Nothing
}

} // namespace BlinKit
