// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: time_ticker.h
// Description: TimeTicker Classes
//      Author: Ziming Li
//     Created: 2021-02-03
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_TIME_TICKER_H
#define BLINKIT_BLINKIT_TIME_TICKER_H

#pragma once

#include "base/time/time.h"

namespace BlinKit {

class TimeTicker
{
public:
    virtual ~TimeTicker(void) = default;

    virtual base::TimeTicks GetTicks(void) = 0;
};

class PerformanceCounterTicker final : public TimeTicker
{
public:
    PerformanceCounterTicker(LONGLONG pcPerSec) : m_pcPerSec(pcPerSec) {}
private:
    base::TimeDelta ToTimeDelta(LONGLONG performanceCount) const;

    base::TimeTicks GetTicks(void) override;

    LONGLONG m_pcPerSec;
};

class MultimediaTicker final : public TimeTicker
{
public:
    MultimediaTicker(void);
    ~MultimediaTicker(void) override;

    void lock(void) { ::WaitForSingleObject(m_hMutex, INFINITE); }
    void unlock(void) { ::ReleaseMutex(m_hMutex); }
private:
    base::TimeTicks GetTicks(void) override;

    HANDLE m_hMutex;
    uint64_t m_rolloverInMs = 0;
    DWORD m_lastTick;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_TIME_TICKER_H
