// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: time_ticker.cpp
// Description: TimeTicker Classes
//      Author: Ziming Li
//     Created: 2021-02-03
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "time_ticker.h"

#include <mutex>
#include <MMSystem.h>

using namespace base;

namespace BlinKit {

MultimediaTicker::MultimediaTicker(void) : m_hMutex(::CreateMutex(nullptr, FALSE, nullptr)), m_lastTick(::timeGetTime())
{
}

MultimediaTicker::~MultimediaTicker(void)
{
    ::CloseHandle(m_hMutex);
}

TimeTicks MultimediaTicker::GetTicks(void)
{
    std::unique_lock<MultimediaTicker> lock(*this);
    DWORD now = ::timeGetTime();
    if (now < m_lastTick)
        m_rolloverInMs += 0x100000000I64;  // ~49.7 days.
    m_lastTick = now;
    return TimeTicks() + TimeDelta::FromMilliseconds(now + m_rolloverInMs);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TimeTicks PerformanceCounterTicker::GetTicks(void)
{
    LARGE_INTEGER pc = { 0 };
    // According to the MSDN documentation for QueryPerformanceCounter(), this
    // will never fail on systems that run XP or later.
    // https://msdn.microsoft.com/library/windows/desktop/ms644904.aspx
    ::QueryPerformanceCounter(&pc);
    return TimeTicks() + ToTimeDelta(pc.QuadPart);
}

TimeDelta PerformanceCounterTicker::ToTimeDelta(LONGLONG performanceCount) const
{
    // If the QPC Value is below the overflow threshold, we proceed with
    // simple multiply and divide.
    if (performanceCount < Time::kQPCOverflowThreshold)
        return TimeDelta::FromMicroseconds(performanceCount * Time::kMicrosecondsPerSecond / m_pcPerSec);

    // Otherwise, calculate microseconds in a round about manner to avoid
    // overflow and precision issues.
    int64_t wholeSeconds = performanceCount / m_pcPerSec;
    int64_t leftoverTicks = performanceCount - (wholeSeconds * m_pcPerSec);
    return TimeDelta::FromMicroseconds((wholeSeconds * Time::kMicrosecondsPerSecond) + ((leftoverTicks * Time::kMicrosecondsPerSecond) / m_pcPerSec));
}

} // namespace BlinKit
