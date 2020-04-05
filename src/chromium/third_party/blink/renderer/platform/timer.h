// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: timer.h
// Description: Timer Classes
//      Author: Ziming Li
//     Created: 2019-11-23
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_TIMER_H
#define BLINKIT_BLINK_TIMER_H

#pragma once

#include "base/location.h"
#include "third_party/blink/renderer/platform/wtf/noncopyable.h"
#include "third_party/blink/renderer/platform/wtf/threading.h"
#include "third_party/blink/renderer/platform/wtf/time.h"

namespace base {
class SingleThreadTaskRunner;
}

namespace blink {

class TimerBase
{
    WTF_MAKE_NONCOPYABLE(TimerBase);
public:
    explicit TimerBase(const std::shared_ptr<base::SingleThreadTaskRunner> &webTaskRunner);
    virtual ~TimerBase(void);

    bool IsActive(void) const { return m_isActive; }

    void Start(TimeDelta nextFireInterval, TimeDelta repeatInterval, const base::Location &caller);

    void StartOneShot(TimeDelta interval, const base::Location &caller) { Start(interval, TimeDelta(), caller); }
private:
    void SetNextFireTime(TimeTicks now, TimeDelta delay);
    void RunInternal(void);
    virtual void Fired(void) = 0;

    std::shared_ptr<bool> m_isAlive;
    bool m_isActive = false;
    base::Location m_location;
    TimeTicks m_nextFireTime;   // 0 if inactive
    TimeDelta m_repeatInterval; // 0 if not repeating
    std::shared_ptr<base::SingleThreadTaskRunner> m_webTaskRunner;

#if DCHECK_IS_ON()
    ThreadIdentifier m_thread;
#endif
};

template <typename TimerFiredClass>
class TaskRunnerTimer : public TimerBase
{
public:
    using TimerFiredFunction = void (TimerFiredClass::*)(TimerBase *);

    TaskRunnerTimer(const std::shared_ptr<base::SingleThreadTaskRunner> &webTaskRunner, TimerFiredClass *o, TimerFiredFunction f)
        : TimerBase(webTaskRunner), m_object(o), m_function(f)
    {
    }
protected:
    void Fired(void) override { (m_object->*m_function)(this); }
private:
    TimerFiredClass *m_object;
    TimerFiredFunction m_function;
};

} // namespace blink

#endif // BLINKIT_BLINK_TIMER_H
