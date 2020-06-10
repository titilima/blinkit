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
#include "base/bit_cast.h"

namespace base {

// Time between resampling the un-granular clock for this API.  60 seconds.
static const int kMaxMillisecondsToAvoidDrift = 60 * Time::kMillisecondsPerSecond;

static int64_t g_initialTime = 0;
static TimeTicks g_initialTicks;

// From MSDN, FILETIME "Contains a 64-bit value representing the number of
// 100-nanosecond intervals since January 1, 1601 (UTC)."
static int64_t FileTimeToMicroseconds(const FILETIME &ft)
{
    // Need to bit_cast to fix alignment, then divide by 10 to convert
    // 100-nanoseconds to microseconds. This only works on little-endian
    // machines.
    return bit_cast<int64_t, FILETIME>(ft) / 10;
}

static int64_t CurrentWallclockMicroseconds(void)
{
    FILETIME ft;
    ::GetSystemTimeAsFileTime(&ft);
    return FileTimeToMicroseconds(ft);
}

static void InitializeClock(void)
{
    g_initialTicks = TimeTicks::Now();
    g_initialTime = CurrentWallclockMicroseconds();
}

Time Time::Now(void)
{
    if (0 == g_initialTime)
        InitializeClock();

    // We implement time using the high-resolution timers so that we can get
    // timeouts which are smaller than 10-15ms.  If we just used
    // CurrentWallclockMicroseconds(), we'd have the less-granular timer.
    //
    // To make this work, we initialize the clock (initial_time) and the
    // counter (initial_ctr).  To compute the initial time, we can check
    // the number of ticks that have elapsed, and compute the delta.
    //
    // To avoid any drift, we periodically resync the counters to the system
    // clock.
    for (;;)
    {
        TimeTicks ticks = TimeTicks::Now();

        // Calculate the time elapsed since we started our timer
        TimeDelta elapsed = ticks - g_initialTicks;

        // Check if enough time has elapsed that we need to resync the clock.
        if (elapsed.InMilliseconds() > kMaxMillisecondsToAvoidDrift)
        {
            InitializeClock();
            continue;
        }

        return Time(elapsed + Time(g_initialTime));
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
