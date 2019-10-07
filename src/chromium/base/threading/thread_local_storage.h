// -------------------------------------------------
// BlinKit - base Library
// -------------------------------------------------
//   File Name: thread_local_storage.h
// Description: Thread Local Storage Classes
//      Author: Ziming Li
//     Created: 2019-09-29
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BLINKIT_BASE_THREAD_LOCAL_STORAGE_H
#define BLINKIT_BASE_THREAD_LOCAL_STORAGE_H

#pragma once

#include "build/build_config.h"

#include <cstdint>
#ifdef OS_WIN
#   include <Windows.h>
#endif
#include "base/macros.h"

namespace base {

namespace internal {

class PlatformThreadLocalStorage
{
public:
#if defined(OS_WIN)
    typedef DWORD   TLSKey;
    enum : unsigned { TLS_KEY_OUT_OF_INDEXES = TLS_OUT_OF_INDEXES };
#endif
    static bool AllocTLS(TLSKey *key);
    static void* GetTLSValue(TLSKey key);
    static void SetTLSValue(TLSKey key, void *value);
};

} // namespace internal

class ThreadLocalStorage
{
public:
    typedef void (*TLSDestructorFunc)(void *value);

    class Slot final
    {
    public:
        explicit Slot(TLSDestructorFunc destructor = nullptr);
        ~Slot(void);

        void* Get(void) const;
        void Set(void *value);
    private:
        static constexpr int kInvalidSlotValue = -1;
        int m_slot = kInvalidSlotValue;
        uint32_t m_version = 0;

        DISALLOW_COPY_AND_ASSIGN(Slot);
    };
};

} // namespace base

#endif // BLINKIT_BASE_THREAD_LOCAL_STORAGE_H
