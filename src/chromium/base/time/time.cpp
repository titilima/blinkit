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

namespace base {
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

bool Time::FromExploded(bool isLocal, const Exploded &exploded, Time *time)
{
    assert(false); // BKTODO:
    return false;
}

double Time::ToDoubleT(void) const
{
    assert(false); // BKTODO:
    return 0;
}

time_t Time::ToTimeT(void) const
{
    assert(false); // BKTODO:
    return 0;
}

TimeDelta TimeDelta::FromMicroseconds(int64_t us)
{
    return TimeDelta(us);
}

TimeDelta TimeDelta::FromMilliseconds(int64_t ms)
{
    return FromProduct(ms, Time::kMicrosecondsPerMillisecond);
}

TimeDelta TimeDelta::FromProduct(int64_t value, int64_t positiveValue)
{
    assert(positiveValue > 0);
    return value > std::numeric_limits<int64_t>::max() / positiveValue
        ? Max()
        : value < std::numeric_limits<int64_t>::min() / positiveValue
        ? Min()
        : TimeDelta(value * positiveValue);
}

TimeDelta TimeDelta::FromSeconds(int64_t secs)
{
    return FromProduct(secs, Time::kMicrosecondsPerSecond);
}

TimeDelta TimeDelta::FromSecondsD(double secs)
{
    assert(false); // BKTODO:
    return TimeDelta();
}

int64_t TimeDelta::InMilliseconds(void) const
{
    return DivideOrMax<int64_t>(Time::kMicrosecondsPerMillisecond);
}

double TimeDelta::InMillisecondsF(void) const
{
    assert(false); // BKTODO:
    return 0;
}

double TimeDelta::InSecondsF(void) const
{
    assert(false); // BKTODO:
    return 0;
}

}  // namespace base
