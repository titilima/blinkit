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

#if defined(OS_WIN)
#   include <Windows.h>
#elif defined(OS_POSIX)
#   include <pthread.h>
#endif

namespace BlinKit {

#if defined(OS_WIN)
class BkSharedMutex
{
public:
    BkSharedMutex(void) { ::InitializeSRWLock(&m_lock); }

    void lock_shared(void) { ::AcquireSRWLockShared(&m_lock); }
    void unlock_shared(void) { ::ReleaseSRWLockShared(&m_lock); }
    void lock(void) { ::AcquireSRWLockExclusive(&m_lock); }
    void unlock(void) { ::ReleaseSRWLockExclusive(&m_lock); }
private:
    SRWLOCK m_lock;
};
#elif defined(OS_POSIX)
class BkSharedMutex
{
public:
    BkSharedMutex(void) { ::pthread_rwlock_init(&m_lock, nullptr); }
    ~BkSharedMutex(void) { ::pthread_rwlock_destroy(&m_lock); }

    void lock_shared(void) { ::pthread_rwlock_rdlock(&m_lock); }
    void unlock_shared(void) { ::pthread_rwlock_unlock(&m_lock); }
    void lock(void) { ::pthread_rwlock_wrlock(&m_lock); }
    void unlock(void) { ::pthread_rwlock_unlock(&m_lock); }
private:
    pthread_rwlock_t m_lock;
};
#endif

}

#endif // BLINKIT_BKCOMMON_BK_SHARED_MUTEX_HPP
