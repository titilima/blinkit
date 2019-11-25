// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: web_task_runner.h
// Description: Task Runner Helpers
//      Author: Ziming Li
//     Created: 2019-10-08
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BLINKIT_BLINK_WEB_TASK_RUNNER_H
#define BLINKIT_BLINK_WEB_TASK_RUNNER_H

#pragma once

#include "base/sequenced_task_runner.h"

namespace blink {

class TaskHandle
{
public:
    ~TaskHandle(void);

    bool IsActive(void) const;
    void Cancel(void);

    class Runner;
private:
    std::shared_ptr<Runner> m_runner;
};

}  // namespace blink

#endif  // BLINKIT_BLINK_WEB_TASK_RUNNER_H
