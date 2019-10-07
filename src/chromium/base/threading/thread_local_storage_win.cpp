// -------------------------------------------------
// BlinKit - base Library
// -------------------------------------------------
//   File Name: thread_local_storage.cpp
// Description: thread_local_storage_win Class
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
    DWORD ret = TlsAlloc();
    if (TLS_OUT_OF_INDEXES != ret)
    {
        *key = ret;
        return true;
    }
    return false;
}

void* PlatformThreadLocalStorage::GetTLSValue(TLSKey key)
{
    return TlsGetValue(key);
}

void PlatformThreadLocalStorage::SetTLSValue(TLSKey key, void *value)
{
    TlsSetValue(key, value);
}

} // namespace internal
} // namespace base
