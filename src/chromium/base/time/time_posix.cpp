// -------------------------------------------------
// BlinKit - base Library
// -------------------------------------------------
//   File Name: time_posix.cpp
// Description: Date & Time Helpers
//      Author: Ziming Li
//     Created: 2020-04-14
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "time.h"

namespace base {

TimeTicks TimeTicks::Now(void)
{
    timespec tp;
    clock_gettime(CLOCK_MONOTONIC, &tp);
    int64_t ms = tp.tv_sec * kMillisecondsPerSecond;
    ms += tp.tv_nsec / 1000000;
    return TimeTicks() + TimeDelta::FromMilliseconds(ms);
}

} // namespace base
