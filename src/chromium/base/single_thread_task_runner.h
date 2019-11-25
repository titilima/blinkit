// -------------------------------------------------
// BlinKit - base Library
// -------------------------------------------------
//   File Name: single_thread_task_runner.h
// Description: SingleThreadTaskRunner Class
//      Author: Ziming Li
//     Created: 2019-10-24
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BASE_SINGLE_THREAD_TASK_RUNNER_H
#define BLINKIT_BASE_SINGLE_THREAD_TASK_RUNNER_H

#pragma once

#include "sequenced_task_runner.h"

namespace base {

class SingleThreadTaskRunner : public SequencedTaskRunner
{
};

} // namespace base 

#endif // BLINKIT_BASE_SINGLE_THREAD_TASK_RUNNER_H
