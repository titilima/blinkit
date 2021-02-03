// -------------------------------------------------
// BlinKit - base Library
// -------------------------------------------------
//   File Name: time_override.h
// Description: Placeholder for Time Overrides
//      Author: Ziming Li
//     Created: 2021-02-03
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BASE_TIME_OVERRIDE_H
#define BLINKIT_BASE_TIME_OVERRIDE_H

#include "base/time/time.h"

namespace base {

using TimeTicksNowFunction = decltype(&TimeTicks::Now);

namespace subtle {
TimeTicks TimeTicksNowIgnoringOverride();
}  // namespace subtle

namespace internal {
extern TimeTicksNowFunction g_timeTicksNowFunction;
} // namespace internal

} // namespace base

#endif // BLINKIT_BASE_TIME_OVERRIDE_H
