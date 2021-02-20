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

#if defined(OS_WIN)
#   include <Windows.h>
#elif defined(OS_POSIX)
#   include <pthread.h>
#endif

namespace BlinKit {

#if defined(OS_WIN)
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
#elif defined(OS_POSIX)
class BkSignal
{
public:
    BkSignal(void)
    {
        if (0 != ::pthread_mutex_init(&m_mutex, nullptr))
            return;
        if (0 == ::pthread_cond_init(&m_cond, nullptr))
            m_valid = true;
        else
            ::pthread_mutex_destroy(&m_mutex);
    }
    ~BkSignal(void)
    {
        if (m_valid)
        {
            ::pthread_cond_destroy(&m_cond);
            ::pthread_mutex_destroy(&m_mutex);
        }
    }

    bool IsValid(void) const { return m_valid; }

    void Wait(void)
    {
        ::pthread_mutex_lock(&m_mutex);
        while (!m_signal)
            ::pthread_cond_wait(&m_cond, &m_mutex);
        m_signal = false;
        ::pthread_mutex_unlock(&m_mutex);
    }
    void Signal(void)
    {
        ::pthread_mutex_lock(&m_mutex);
        m_signal = true;
        ::pthread_mutex_unlock(&m_mutex);
        ::pthread_cond_signal(&m_cond);
    }
private:
    pthread_mutex_t m_mutex;
    bool m_signal = false;
    bool m_valid = false;
    pthread_cond_t m_cond;
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

    bool IsSignalValid(void) const { return m_signal.IsValid(); }
    void Signal(void) { m_signal.Signal(); }
private:
    BkSignal m_signal;
};

}

#endif // BLINKIT_BKCOMMON_BK_SIGNAL_HPP
