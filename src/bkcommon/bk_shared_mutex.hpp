// -------------------------------------------------
// BlinKit - BkCommon Library
// -------------------------------------------------
//   File Name: bk_shared_mutex.hpp
// Description: BkSharedMutex Class
//      Author: Ziming Li
//     Created: 2021-01-25
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BKCOMMON_BK_SHARED_MUTEX_HPP
#define BLINKIT_BKCOMMON_BK_SHARED_MUTEX_HPP

#pragma once

#include <mutex>
#include <shared_mutex>

namespace BlinKit {

#ifdef OS_WIN
class BkSharedMutex
{
public:
    BkSharedMutex(void);

    void lock_shared(void) { ::AcquireSRWLockShared(&m_lock); }
    void unlock_shared(void) { ::ReleaseSRWLockShared(&m_lock); }
    void lock(void) { ::AcquireSRWLockExclusive(&m_lock); }
    void unlock(void) { ::ReleaseSRWLockExclusive(&m_lock); }
private:
    SRWLOCK m_lock;
};
#endif

}

#endif // BLINKIT_BKCOMMON_BK_SHARED_MUTEX_HPP
