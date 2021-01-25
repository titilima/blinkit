// -------------------------------------------------
// BlinKit - BkCommon Library
// -------------------------------------------------
//   File Name: bk_mutex.hpp
// Description: BkMutex Class
//      Author: Ziming Li
//     Created: 2021-01-25
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BKCOMMON_BK_MUTEX_HPP
#define BLINKIT_BKCOMMON_BK_MUTEX_HPP

#pragma once

#include <mutex>

namespace BlinKit {

#ifdef OS_WIN
class BkMutex
{
public:
    BkMutex(void) : m_mutex(::CreateMutex(nullptr, FALSE, nullptr)) {}
    ~BkMutex(void) { ::CloseHandle(m_mutex); }

    void lock(void) { ::WaitForSingleObject(m_mutex, INFINITE); }
    void unlock(void) { ::ReleaseMutex(m_mutex); }
private:
    HANDLE m_mutex;
};
#endif

}

#endif // BLINKIT_BKCOMMON_BK_MUTEX_HPP
