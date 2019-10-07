// -------------------------------------------------
// BlinKit - base Library
// -------------------------------------------------
//   File Name: thread_local_storage.cpp
// Description: Thread Local Storage Classes
//      Author: Ziming Li
//     Created: 2019-09-29
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "thread_local_storage.h"

#include <mutex>

namespace base {

using namespace internal;

static PlatformThreadLocalStorage::TLSKey g_nativeTlsKey = PlatformThreadLocalStorage::TLS_KEY_OUT_OF_INDEXES;

void* const kUninitialized = nullptr;
void* const kDestroyed = reinterpret_cast<void*>(1);

constexpr int kThreadLocalStorageSize = 256;

enum TlsStatus {
    FREE,
    IN_USE,
};

struct TlsMetadata {
    TlsStatus status;
    ThreadLocalStorage::TLSDestructorFunc destructor;
    uint32_t version;
};

struct TlsVectorEntry {
    void *data;
    uint32_t version;
};

static std::mutex g_metadataLock;
static TlsMetadata g_tlsMetadata[kThreadLocalStorageSize];
static size_t g_lastAssignedSlot = 0;

static TlsVectorEntry* EnsureTlsVector(void)
{
    assert(PlatformThreadLocalStorage::TLS_KEY_OUT_OF_INDEXES != g_nativeTlsKey);

    void *p = PlatformThreadLocalStorage::GetTLSValue(g_nativeTlsKey);
    if (kUninitialized == p)
    {
        TlsVectorEntry *vectorData = new TlsVectorEntry[kThreadLocalStorageSize];
        memset(vectorData, 0, sizeof(TlsVectorEntry) * kThreadLocalStorageSize);
        PlatformThreadLocalStorage::SetTLSValue(g_nativeTlsKey, vectorData);

        p = vectorData;
    }
    return reinterpret_cast<TlsVectorEntry *>(p);
}

ThreadLocalStorage::Slot::Slot(TLSDestructorFunc destructor)
{
    static std::once_flag s_flag;
    const auto initializer = []
    {
        PlatformThreadLocalStorage::AllocTLS(&g_nativeTlsKey);
        assert(PlatformThreadLocalStorage::TLS_KEY_OUT_OF_INDEXES != g_nativeTlsKey);
    };
    std::call_once(s_flag, initializer);

    EnsureTlsVector();

    // Grab a new slot.
    std::lock_guard<std::mutex> lock(g_metadataLock);
    for (int i = 0; i < kThreadLocalStorageSize; ++i)
    {
        // Tracking the last assigned slot is an attempt to find the next
        // available slot within one iteration. Under normal usage, slots remain
        // in use for the lifetime of the process (otherwise before we reclaimed
        // slots, we would have run out of slots). This makes it highly likely the
        // next slot is going to be a free slot.
        size_t slotCandidate = (g_lastAssignedSlot + 1 + i) % kThreadLocalStorageSize;
        if (g_tlsMetadata[slotCandidate].status == TlsStatus::FREE)
        {
            g_tlsMetadata[slotCandidate].status = TlsStatus::IN_USE;
            g_tlsMetadata[slotCandidate].destructor = destructor;
            g_lastAssignedSlot = slotCandidate;
            m_slot = slotCandidate;
            m_version = g_tlsMetadata[slotCandidate].version;
            return;
        }
    }
}

ThreadLocalStorage::Slot::~Slot(void)
{
    assert(false); // BKTODO:
}

void* ThreadLocalStorage::Slot::Get(void) const
{
    const TlsVectorEntry *vectorData = reinterpret_cast<TlsVectorEntry *>(PlatformThreadLocalStorage::GetTLSValue(g_nativeTlsKey));
    assert(kDestroyed != vectorData);
    if (nullptr == vectorData)
        return nullptr;

    assert(kInvalidSlotValue != m_slot);
    assert(m_slot < kThreadLocalStorageSize);
    // Version mismatches means this slot was previously freed.
    if (vectorData[m_slot].version != m_version)
        return nullptr;

    return vectorData[m_slot].data;
}

void ThreadLocalStorage::Slot::Set(void *value)
{
    TlsVectorEntry *vectorData = EnsureTlsVector();

    assert(kInvalidSlotValue != m_slot);
    assert(m_slot < kThreadLocalStorageSize);
    vectorData[m_slot].data = value;
    vectorData[m_slot].version = m_version;
}

} // namespace base
