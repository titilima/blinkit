// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: timer.cpp
// Description: Timer Classes
//      Author: Ziming Li
//     Created: 2019-11-23
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "timer.h"

#include "base/single_thread_task_runner.h"
#include "third_party/blink/public/platform/platform.h"

namespace blink {

TimerBase::TimerBase(const std::shared_ptr<base::SingleThreadTaskRunner> &webTaskRunner)
    : m_isAlive(std::make_shared<bool>(true))
    , m_webTaskRunner(webTaskRunner)
#if DCHECK_IS_ON()
    , m_thread(CurrentThread())
#endif
{
}

TimerBase::~TimerBase(void)
{
    *m_isAlive = false;
}

void TimerBase::RunInternal(void)
{
    m_isActive = false;

#if DCHECK_IS_ON()
    ASSERT(CurrentThread() == m_thread);
#endif

    if (!m_repeatInterval.is_zero())
    {
        TimeTicks now = base::TimeTicks::Now();
        // This computation should be drift free, and it will cope if we miss a
        // beat, which can easily happen if the thread is busy.  It will also cope
        // if we get called slightly before m_unalignedNextFireTime, which can
        // happen due to lack of timer precision.
        TimeDelta intervalToNextFireTime = m_repeatInterval - (now - m_nextFireTime) % m_repeatInterval;
        SetNextFireTime(now, intervalToNextFireTime);
    }
    else
    {
        m_nextFireTime = TimeTicks();
    }
    Fired();
}

void TimerBase::SetNextFireTime(TimeTicks now, TimeDelta delay)
{
#if DCHECK_IS_ON()
    ASSERT(CurrentThread() == m_thread);
#endif

    TimeTicks newTime = now + delay;

    if (m_nextFireTime != newTime)
    {
        m_nextFireTime = newTime;

        std::shared_ptr<bool> isAlive(m_isAlive);
        auto callback = [this, isAlive]
        {
            if (*isAlive)
                RunInternal();
        };
        m_isActive = true;
        m_webTaskRunner->PostDelayedTask(m_location, callback, delay);
    }
}

void TimerBase::Start(TimeDelta nextFireInterval, TimeDelta repeatInterval, const base::Location &caller)
{
#if DCHECK_IS_ON()
    ASSERT(CurrentThread() == m_thread);
#endif

    m_location = caller;
    m_repeatInterval = repeatInterval;
    SetNextFireTime(base::TimeTicks::Now(), nextFireInterval);
}

void TimerBase::Stop(void)
{
    ASSERT(CurrentThread() == m_thread);

    m_repeatInterval = TimeDelta();
    m_nextFireTime = TimeTicks();
    *m_isAlive = false;
}

} // namespace blink
