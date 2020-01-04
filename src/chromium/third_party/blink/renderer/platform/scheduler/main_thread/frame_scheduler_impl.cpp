// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: frame_scheduler_impl.cpp
// Description: FrameSchedulerImpl Class
//      Author: Ziming Li
//     Created: 2019-12-05
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "frame_scheduler_impl.h"

#include "base/memory/ptr_util.h"
#include "third_party/blink/public/platform/platform.h"

namespace blink {
namespace scheduler {

FrameSchedulerImpl::FrameSchedulerImpl(void) = default;

FrameSchedulerImpl::~FrameSchedulerImpl(void) = default;

std::unique_ptr<FrameSchedulerImpl> FrameSchedulerImpl::Create(void)
{
    return base::WrapUnique(new FrameSchedulerImpl);
}

std::shared_ptr<base::SingleThreadTaskRunner> FrameSchedulerImpl::GetTaskRunner(TaskType type)
{
    return Platform::Current()->CurrentThread()->GetTaskRunner();
}

} // namespace scheduler
} // namespace blink
