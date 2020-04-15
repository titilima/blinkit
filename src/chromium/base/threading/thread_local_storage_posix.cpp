// -------------------------------------------------
// BlinKit - base Library
// -------------------------------------------------
//   File Name: thread_local_storage_posix.cpp
// Description: PlatformThreadLocalStorage Class
//      Author: Ziming Li
//     Created: 2019-10-07
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "thread_local_storage.h"

namespace base {
namespace internal {

bool PlatformThreadLocalStorage::AllocTLS(TLSKey *key)
{
    return 0 == pthread_key_create(key, OnThreadExit);
}

void PlatformThreadLocalStorage::FreeTLS(TLSKey key)
{
    int ret = pthread_key_delete(key);
    assert(0 == ret);
}

void* PlatformThreadLocalStorage::GetTLSValue(TLSKey key)
{
    return pthread_getspecific(key);
}

void PlatformThreadLocalStorage::OnThreadExit(void *value)
{
    assert(false); // BKTODO:
}

void PlatformThreadLocalStorage::SetTLSValue(TLSKey key, void *value)
{
    int ret = pthread_setspecific(key, value);
    assert(0 == ret);
}

} // namespace internal
} // namespace base
