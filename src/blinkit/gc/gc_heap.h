// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: gc_heap.h
// Description: GCHeap Class
//      Author: Ziming Li
//     Created: 2020-11-14
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_GC_HEAP_H
#define BLINKIT_BLINKIT_GC_HEAP_H

#pragma once

#include <stack>
#include "blinkit/gc/lifecycle_data_manager.h"

namespace BlinKit {

struct GCObjectHeader;
class GCVisitor;

class GCHeap
{
public:
    static void Initialize(void);
    static void Finalize(void);

#ifndef NDEBUG
    static void TrackObject(GCObject &o);
    static void UntrackObject(GCObject &o);
#endif

    void AddGlobalFinalizer(const std::function<void()> &finalizer) { m_globalFinalizers.emplace(finalizer); }

    static void RegisterLifecycleObserver(GCObject *o, GCLifecycleObserver *ob);
    static void RemoveLifecycleObserverForObject(GCObject *o, GCLifecycleObserver *ob);
    static void RemoveLifecycleObserver(GCLifecycleObserver *ob);

    void AttachWeakSlot(GCObject &o, void **slot) { m_lifecycleDataManager.AttachWeakSlot(&o, slot); }
    void DetachWeakSlot(GCObject &o, void **slot) { m_lifecycleDataManager.DetachWeakSlot(&o, slot); }
    static void ProcessObjectFinalizing(GCObject &o);
private:
    GCHeap(void);
    ~GCHeap(void);

    void CleanupGlobalObjects(void);

#ifndef NDEBUG
    std::unordered_set<GCObject *> m_allObjects;
#endif
    LifecycleDataManager m_lifecycleDataManager;
    std::stack<std::function<void()>> m_globalFinalizers;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_GC_HEAP_H
