// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: gc_lifecycle_observer.cpp
// Description: LifecycleDataManager Class
//      Author: Ziming Li
//     Created: 2021-08-19
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./gc_def.h"

#include "blinkit/gc/gc_heap.h"

namespace BlinKit {

GCLifecycleObserver::~GCLifecycleObserver(void)
{
    GCHeap::RemoveLifecycleObserver(this);
}

void GCLifecycleObserver::Observe(GCObject *o)
{
    GCHeap::RegisterLifecycleObserver(o, this);
}

void GCLifecycleObserver::Unobserve(GCObject *o)
{
    GCHeap::RemoveLifecycleObserverForObject(o, this);
}

} // namespace BlinKit
