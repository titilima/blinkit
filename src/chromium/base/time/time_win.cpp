// -------------------------------------------------
// BlinKit - base Library
// -------------------------------------------------
//   File Name: time_win.cpp
// Description: Date & Time Helpers
//      Author: Ziming Li
//     Created: 2018-08-17
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "time.h"

#include <mutex>
#include <Windows.h>

namespace base {

static std::mutex g_rolloverLock;
static int64_t g_rolloverMs = 0;
static DWORD g_lastSeenNow = 0;

TimeTicks TimeTicks::Now(void)
{
    std::lock_guard<std::mutex> locked(g_rolloverLock);
    DWORD now = timeGetTime();
    if (now < g_lastSeenNow)
        g_rolloverMs += 0x100000000I64;  // ~49.7 days.
    g_lastSeenNow = now;
    return TimeTicks() + TimeDelta::FromMilliseconds(now + g_rolloverMs);
}

}  // namespace base
