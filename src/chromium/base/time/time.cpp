// -------------------------------------------------
// BlinKit - base Library
// -------------------------------------------------
//   File Name: time.cpp
// Description: Date & Time Helpers
//      Author: Ziming Li
//     Created: 2019-03-09
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "time.h"

#include "base/numerics/checked_math.h"
#include "base/time/time_override.h"

namespace base {

namespace internal {
TimeTicksNowFunction g_timeTicksNowFunction = &subtle::TimeTicksNowIgnoringOverride;
} // namespace internal

namespace time_internal {

int64_t SaturatedAdd(TimeDelta delta, int64_t value)
{
    CheckedNumeric<int64_t> rv(delta.m_delta);
    rv += value;
    if (rv.IsValid())
        return rv.ValueOrDie();
    // Positive RHS overflows. Negative RHS underflows.
    if (value < 0)
        return std::numeric_limits<int64_t>::min();
    return std::numeric_limits<int64_t>::max();
}

int64_t SaturatedSub(TimeDelta delta, int64_t value)
{
    CheckedNumeric<int64_t> rv(delta.m_delta);
    rv -= value;
    if (rv.IsValid())
        return rv.ValueOrDie();
    // Negative RHS overflows. Positive RHS underflows.
    if (value < 0)
        return std::numeric_limits<int64_t>::max();
    return std::numeric_limits<int64_t>::min();
}

} // namespace time_internal

bool Time::ExplodedMostlyEquals(const Exploded &lhs, const Exploded &rhs)
{
    return lhs.year == rhs.year && lhs.month == rhs.month && lhs.day_of_month == rhs.day_of_month
        && lhs.hour == rhs.hour && lhs.minute == rhs.minute && lhs.second == rhs.second
        && lhs.millisecond == rhs.millisecond;
}

double Time::ToDoubleT(void) const
{
    if (is_null())
        return 0.0;  // Preserve 0 so we can tell it doesn't exist.
    if (is_max())
        return std::numeric_limits<double>::infinity(); // Preserve max without offset to prevent overflow.
    return (static_cast<double>(m_us - kTimeTToMicrosecondsOffset) / static_cast<double>(kMicrosecondsPerSecond));
}

time_t Time::ToTimeT(void) const
{
    ASSERT(false); // BKTODO:
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TimeDelta TimeDelta::FromMicroseconds(int64_t us)
{
    return TimeDelta(us);
}

TimeDelta TimeDelta::FromSeconds(int64_t secs)
{
    return FromProduct(secs, Time::kMicrosecondsPerSecond);
}

int64_t TimeDelta::InMicroseconds(void) const
{
    return DivideOrMax<int64_t>(1);
}

int64_t TimeDelta::InMilliseconds(void) const
{
    return DivideOrMax<int64_t>(Time::kMicrosecondsPerMillisecond);
}

double TimeDelta::InMillisecondsF(void) const
{
    return DivideOrMax<double>(Time::kMicrosecondsPerMillisecond);
}

double TimeDelta::InSecondsF(void) const
{
    return DivideOrMax<double>(Time::kMicrosecondsPerSecond);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TimeTicks TimeTicks::Now(void)
{
    return internal::g_timeTicksNowFunction();
}

}  // namespace base
