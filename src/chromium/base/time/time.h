// -------------------------------------------------
// BlinKit - base Library
// -------------------------------------------------
//   File Name: time.h
// Description: Date & Time Helpers
//      Author: Ziming Li
//     Created: 2019-03-09
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BLINKIT_BASE_TIME_H
#define BLINKIT_BASE_TIME_H

#pragma once

#include "build/build_config.h"

#include <cstdint>
#include <limits>

namespace base {

class TimeDelta;

namespace time_internal {
int64_t SaturatedAdd(TimeDelta delta, int64_t value);
} // namespace time_internal

/**
 * TimeDelta
 */
class TimeDelta
{
public:
    constexpr TimeDelta(void) = default;

    static TimeDelta FromSecondsD(double secs);
    static TimeDelta FromMilliseconds(int64_t ms);
    static TimeDelta FromMicroseconds(int64_t us);

    double InSecondsF(void) const;
    double InMillisecondsF(void) const;

    static constexpr TimeDelta Max(void) {
        return TimeDelta(std::numeric_limits<int64_t>::max());
    }
    static constexpr TimeDelta Min(void) {
        return TimeDelta(std::numeric_limits<int64_t>::min());
    }
private:
    friend int64_t time_internal::SaturatedAdd(TimeDelta delta, int64_t value);

    constexpr explicit TimeDelta(int64_t deltaUs) : m_delta(deltaUs) {}

    static TimeDelta FromProduct(int64_t value, int64_t positiveValue);

    int64_t m_delta = 0; // In microseconds
};

namespace time_internal {

template<class T>
class TimeBase
{
public:
    static constexpr int64_t kMillisecondsPerSecond = 1000;
    static constexpr int64_t kMicrosecondsPerMillisecond = 1000;

    TimeDelta since_origin(void) const {
        return TimeDelta::FromMicroseconds(m_us);
    }

    T operator+(TimeDelta delta) const {
        return T(time_internal::SaturatedAdd(delta, m_us));
    }
    TimeDelta operator-(T other) const {
        return TimeDelta::FromMicroseconds(m_us - other.m_us);
    }
protected:
    explicit TimeBase(int64_t us) : m_us(us) {}
private:
    int64_t m_us;
};

} // namespace time_internal

/**
 * Time
 */
class Time : public time_internal::TimeBase<Time>
{
public:
    Time(void) : TimeBase(0) {}

    static Time Now(void);
    double ToDoubleT(void) const;
};

/**
 * TimeTicks
 */
class TimeTicks : public time_internal::TimeBase<TimeTicks>
{
public:
    TimeTicks(void) : TimeBase(0) {}

    static TimeTicks Now(void);
private:
    friend class time_internal::TimeBase<TimeTicks>;

    explicit TimeTicks(int64_t us) : TimeBase(us) {}
};

}  // namespace base

#endif  // BLINKIT_BASE_TIME_H
