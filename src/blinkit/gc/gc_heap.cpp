// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: gc_heap.cpp
// Description: GCHeap Class
//      Author: Ziming Li
//     Created: 2020-11-14
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "./gc_heap.h"

#include "blinkit/blink/renderer/wtf/MainThread.h"
#include "blinkit/gc/garbage_collector.h"
#include "third_party/zed/include/zed/container_utilites.hpp"

namespace BlinKit {

static GCHeap *theHeap = nullptr;

GCHeap::GCHeap(void)
{
    ASSERT(nullptr == theHeap);
    theHeap = this;
}

GCHeap::~GCHeap(void)
{
    ASSERT(this == theHeap);
    ASSERT(isMainThread());

    CleanupGlobalObjects();
    ASSERT(m_allObjects.empty());

    theHeap = nullptr;
}

void GCHeap::CleanupGlobalObjects(void)
{
    while (!m_globalFinalizers.empty())
    {
        m_globalFinalizers.top()();
        m_globalFinalizers.pop();
    }
}

void GCHeap::Finalize(void)
{
    delete theHeap;
}

void GCHeap::Initialize(void)
{
    new GCHeap;
}

void GCHeap::ProcessObjectFinalizing(GCObject &o)
{
    theHeap->m_lifecycleDataManager.ProcessObjectFinalizing(&o);
}

void GCHeap::RegisterLifecycleObserver(GCObject *o, GCLifecycleObserver *ob)
{
    theHeap->m_lifecycleDataManager.RegisterObserver(o, ob);
}

void GCHeap::RemoveLifecycleObserver(GCLifecycleObserver *ob)
{
    theHeap->m_lifecycleDataManager.RemoveObserver(ob);
}

void GCHeap::RemoveLifecycleObserverForObject(GCObject *o, GCLifecycleObserver *ob)
{
    theHeap->m_lifecycleDataManager.RemoveObserverForObject(o, ob);
}

#ifndef NDEBUG
void GCHeap::TrackObject(GCObject &o)
{
    theHeap->m_allObjects.emplace(&o);
}

void GCHeap::UntrackObject(GCObject &o)
{
    theHeap->m_allObjects.erase(&o);
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GCAddGlobalFinalizer(const std::function<void()> &finalizer)
{
    theHeap->AddGlobalFinalizer(finalizer);
}

void GCAttachWeakSlot(GCObject *o, void **weakSlot)
{
    if (nullptr != o)
        theHeap->AttachWeakSlot(*o, weakSlot);
}

void GCDetachWeakSlot(GCObject *o, void **weakSlot)
{
    if (nullptr != o)
        theHeap->DetachWeakSlot(*o, weakSlot);
}

} // namespace BlinKit
