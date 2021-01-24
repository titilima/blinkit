// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: scoped_event_waiter.h
// Description: ScopedEventWaiter Class
//      Author: Ziming Li
//     Created: 2021-01-24
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_SCOPED_EVENT_WAITER_H
#define BLINKIT_BLINKIT_SCOPED_EVENT_WAITER_H

#pragma once

namespace BlinKit {

class ScopedEventWaiter
{
public:
    ScopedEventWaiter(void) : m_hEvent(::CreateEvent(nullptr, FALSE, FALSE, nullptr))
    {
        ASSERT(nullptr != m_hEvent);
    }
    ~ScopedEventWaiter(void)
    {
        if (nullptr != m_hEvent)
        {
            WaitForSingleObject(m_hEvent, INFINITE);
            CloseHandle(m_hEvent);
        }
    }

    operator bool() const { return nullptr != m_hEvent; }

    void Signal(void) { ::SetEvent(m_hEvent); }
private:
    HANDLE m_hEvent;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_SCOPED_EVENT_WAITER_H
