// -------------------------------------------------
// BlinKit - BkCommon Library
// -------------------------------------------------
//   File Name: bk_signal.hpp
// Description: BkSignal Classes
//      Author: Ziming Li
//     Created: 2021-01-25
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BKCOMMON_BK_SIGNAL_HPP
#define BLINKIT_BKCOMMON_BK_SIGNAL_HPP

#pragma once

namespace BlinKit {

#ifdef OS_WIN
class BkSignal
{
public:
    BkSignal(void) : m_hEvent(::CreateEvent(nullptr, FALSE, FALSE, nullptr)) {}
    ~BkSignal(void) { ::CloseHandle(m_hEvent); }

    bool IsValid(void) const { return nullptr != m_hEvent; }

    void Wait(void) { ::WaitForSingleObject(m_hEvent, INFINITE); }
    void Signal(void) { ::SetEvent(m_hEvent); }
private:
    HANDLE m_hEvent;
};
#endif

class ScopedSignalWaiter
{
public:
    ~ScopedSignalWaiter(void)
    {
        if (m_signal.IsValid())
            m_signal.Wait();
    }

    void Signal(void) { m_signal.Signal(); }
private:
    BkSignal m_signal;
};

}

#endif // BLINKIT_BKCOMMON_BK_SIGNAL_HPP
