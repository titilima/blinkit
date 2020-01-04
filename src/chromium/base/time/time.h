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
int64_t SaturatedSub(TimeDelta delta, int64_t value);
} // namespace time_internal

/**
 * TimeDelta
 */
class TimeDelta
{
public:
    constexpr TimeDelta(void) = default;

    static TimeDelta FromSeconds(int64_t secs);
    static TimeDelta FromSecondsD(double secs);
    static TimeDelta FromMilliseconds(int64_t ms);
    static TimeDelta FromMicroseconds(int64_t us);

    double InSecondsF(void) const;
    int64_t InMilliseconds(void) const;
    double InMillisecondsF(void) const;

    static constexpr TimeDelta Max(void) {
        return TimeDelta(std::numeric_limits<int64_t>::max());
    }
    static constexpr TimeDelta Min(void) {
        return TimeDelta(std::numeric_limits<int64_t>::min());
    }

    constexpr bool is_zero(void) const { return 0 == m_delta; }
    constexpr bool is_max(void) const { return std::numeric_limits<int64_t>::max() == m_delta; }
    constexpr bool is_min(void) const { return std::numeric_limits<int64_t>::min() == m_delta; }

    TimeDelta operator-(TimeDelta other) const {
        return TimeDelta(time_internal::SaturatedSub(*this, other.m_delta));
    }
    constexpr TimeDelta operator%(TimeDelta a) const {
        return TimeDelta(m_delta % a.m_delta);
    }
private:
    friend int64_t time_internal::SaturatedAdd(TimeDelta delta, int64_t value);
    friend int64_t time_internal::SaturatedSub(TimeDelta delta, int64_t value);

    constexpr explicit TimeDelta(int64_t deltaUs) : m_delta(deltaUs) {}

    static TimeDelta FromProduct(int64_t value, int64_t positiveValue);

    template <typename T>
    constexpr T DivideOrMax(int64_t divisor) const {
        return is_max() ? std::numeric_limits<T>::max()
            : static_cast<T>(m_delta / divisor);
    }

    int64_t m_delta = 0; // In microseconds
};

namespace time_internal {

template<class T>
class TimeBase
{
public:
    static constexpr int64_t kMillisecondsPerSecond = 1000;
    static constexpr int64_t kMicrosecondsPerMillisecond = 1000;
    static constexpr int64_t kMicrosecondsPerSecond = kMicrosecondsPerMillisecond * kMillisecondsPerSecond;

    constexpr bool is_null(void) const { return 0 == m_us; }

    TimeDelta since_origin(void) const {
        return TimeDelta::FromMicroseconds(m_us);
    }

    T operator+(TimeDelta delta) const {
        return T(time_internal::SaturatedAdd(delta, m_us));
    }
    TimeDelta operator-(T other) const {
        return TimeDelta::FromMicroseconds(m_us - other.m_us);
    }

    // Comparison operators
    bool operator==(const T &other) const {
        return m_us == other.m_us;
    }
    bool operator!=(const T &other) const {
        return m_us != other.m_us;
    }
    bool operator<(const T &other) const {
        return m_us < other.m_us;
    }
    bool operator<=(const T &other) const {
        return m_us <= other.m_us;
    }
    bool operator>(const T &other) const {
        return m_us > other.m_us;
    }
    bool operator>=(const T &other) const {
        return m_us >= other.m_us;
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
    // Represents an exploded time that can be formatted nicely. This is kind of
    // like the Win32 SYSTEMTIME structure or the Unix "struct tm" with a few
    // additions and changes to prevent errors.
    struct Exploded {
        int year;          // Four digit year "2007"
        int month;         // 1-based month (values 1 = January, etc.)
        int day_of_week;   // 0-based day of week (0 = Sunday, etc.)
        int day_of_month;  // 1-based day of month (1-31)
        int hour;          // Hour within the current day (0-23)
        int minute;        // Minute within the current hour (0-59)
        int second;        // Second within the current minute (0-59 plus leap
                           //   seconds which may take it up to 60).
        int millisecond;   // Milliseconds within the current second (0-999)

        // A cursory test for whether the data members are within their
        // respective ranges. A 'true' return value does not guarantee the
        // Exploded value can be successfully converted to a Time value.
        bool HasValidValues(void) const;
    };

    Time(void) : TimeBase(0) {}

    static Time Now(void);
    double ToDoubleT(void) const;
    time_t ToTimeT(void) const;

    // Converts an exploded structure representing either the local time or UTC
    // into a Time class. Returns false on a failure when, for example, a day of
    // month is set to 31 on a 28-30 day month. Returns Time(0) on overflow.
    static Time FromUTCExploded(const Exploded &exploded) {
        Time t;
        FromExploded(false, exploded, &t);
        return t;
    }

private:
    friend class time_internal::TimeBase<Time>;

    explicit Time(int64_t us) : TimeBase(us) {}

    // Unexplodes a given time assuming the source is either local time
    // |is_local = true| or UTC |is_local = false|. Function returns false on
    // failure and sets |time| to Time(0). Otherwise returns true and sets |time|
    // to non-exploded time.
    static bool FromExploded(bool isLocal, const Exploded &exploded, Time *time);
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
