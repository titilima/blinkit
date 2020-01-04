// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: frame_scheduler.h
// Description: FrameScheduler Class
//      Author: Ziming Li
//     Created: 2019-12-05
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BLINKIT_BLINK_FRAME_SCHEDULER_H
#define BLINKIT_BLINK_FRAME_SCHEDULER_H

#pragma once

#include "third_party/blink/public/platform/task_type.h"

namespace base {
class SingleThreadTaskRunner;
}

namespace blink {

class FrameScheduler
{
public:
    virtual ~FrameScheduler(void) = default;

    virtual std::shared_ptr<base::SingleThreadTaskRunner> GetTaskRunner(TaskType type) = 0;
};

} // namespace blink

#endif // BLINKIT_BLINK_FRAME_SCHEDULER_H
