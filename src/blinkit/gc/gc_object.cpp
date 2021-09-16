// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: gc_object.cpp
// Description: GCObject Class
//      Author: Ziming Li
//     Created: 2021-08-10
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./gc_def.h"

#include "blinkit/blink/renderer/wtf/MainThread.h"
#include "blinkit/gc/garbage_collector.h"
#include "blinkit/gc/gc_heap.h"

namespace BlinKit {

GCObject::GCObject(void)
{
#ifndef NDEBUG
    GCHeap::TrackObject(*this);
#endif
}

GCObject::~GCObject(void)
{
    GCHeap::ProcessObjectFinalizing(*this);
#ifndef NDEBUG
    GCHeap::UntrackObject(*this);
#endif
}

unsigned GCObject::DecRef(void)
{
    ASSERT(isMainThread());
    return --m_refCnt;
}

void GCObject::IncRef(void)
{
    ASSERT(isMainThread());
    ++m_refCnt;
}

void GCObject::Release(GCOption option)
{
    if (DecRef() > 0)
    {
        if (GCOption::Auto == option)
            return;
    }

    ASSERT(0 == m_refCnt || GCOption::Full == option);
    GarbageCollector::PerformOnMember(*this);
}

} // namespace BlinKit
