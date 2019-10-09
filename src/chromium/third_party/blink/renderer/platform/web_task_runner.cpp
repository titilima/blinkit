// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: web_task_runner.cpp
// Description: Task Runner Helpers
//      Author: Ziming Li
//     Created: 2019-10-08
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "web_task_runner.h"

namespace blink {

class TaskHandle::Runner : public std::enable_shared_from_this<TaskHandle::Runner>
{
public:
    bool IsActive(void) const
    {
        assert(false); // BKTODO:
        return false;
    }
    void Cancel(void)
    {
        assert(false); // BKTODO:
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TaskHandle::~TaskHandle(void)
{
    Cancel();
}

void TaskHandle::Cancel(void)
{
    if (m_runner)
    {
        m_runner->Cancel();
        m_runner.reset();
    }
}

bool TaskHandle::IsActive(void) const
{
    return m_runner && m_runner->IsActive();
}

}  // namespace blink
