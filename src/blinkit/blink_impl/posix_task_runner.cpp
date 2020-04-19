// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: posix_task_runner.cpp
// Description: PosixTaskRunner Class
//      Author: Ziming Li
//     Created: 2020-04-19
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "posix_task_runner.h"

namespace BlinKit {

bool PosixThreadTaskRunner::PostDelayedTask(const base::Location &fromHere, const std::function<void()> &task, base::TimeDelta delay)
{
    ASSERT(false); // BKTODO:
    return true;
}

} // namespace BlinKit
