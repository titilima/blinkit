// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: run_loop.h
// Description: RunLoop Class
//      Author: Ziming Li
//     Created: 2021-02-20
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_RUN_LOOP_H
#define BLINKIT_BLINKIT_RUN_LOOP_H

#pragma once

#include "base/single_thread_task_runner.h"

namespace BlinKit {

class RunLoop
{
public:
    std::shared_ptr<base::SingleThreadTaskRunner> GetTaskRunner(void) const;

    int Run(void);
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_RUN_LOOP_H
