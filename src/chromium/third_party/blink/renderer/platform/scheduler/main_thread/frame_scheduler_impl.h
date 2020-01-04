// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: frame_scheduler_impl.h
// Description: FrameSchedulerImpl Class
//      Author: Ziming Li
//     Created: 2019-12-05
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BLINKIT_BLINK_FRAME_SCHEDULER_IMPL_H
#define BLINKIT_BLINK_FRAME_SCHEDULER_IMPL_H

#pragma once

#include "third_party/blink/renderer/platform/scheduler/public/frame_scheduler.h"

namespace blink {
namespace scheduler {

class FrameSchedulerImpl : public FrameScheduler
{
public:
    static std::unique_ptr<FrameSchedulerImpl> Create(void);
    ~FrameSchedulerImpl(void) override;

    std::shared_ptr<base::SingleThreadTaskRunner> GetTaskRunner(TaskType type) override;
protected:
    FrameSchedulerImpl(void);

    DISALLOW_COPY_AND_ASSIGN(FrameSchedulerImpl);
};

} // namespace scheduler
} // namespace blink

#endif // BLINKIT_BLINK_FRAME_SCHEDULER_IMPL_H
