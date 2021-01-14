// -------------------------------------------------
// BlinKit - base Library
// -------------------------------------------------
//   File Name: task_runner.cpp
// Description: TaskRunner Class
//      Author: Ziming Li
//     Created: 2019-10-24
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "task_runner.h"

namespace base {

bool TaskRunner::PostTask(const Location &fromHere, std::function<void()> &&task)
{
    return PostDelayedTask(fromHere, std::move(task), TimeDelta());
}

} // namespace base
