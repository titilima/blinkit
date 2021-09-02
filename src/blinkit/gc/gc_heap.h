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

#include "blinkit/gc/lifecycle_data_manager.h"

namespace BlinKit {

struct GCObjectHeader;
class GCVisitor;

class GCHeap
{
public:
    static void Initialize(void);
    static void Finalize(void);

    void SetGlobalObject(GCObject &o);
    void RetainPersistentObject(GCObject &o, void **slot);
    void ReleasePersistentObject(GCObject &o, void **slot);

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
    void CleanupPersistentObjects(void);

    LifecycleDataManager m_lifecycleDataManager;
    std::unordered_set<GCObject *> m_globalObjects;
    std::unordered_map<GCObject *, std::unordered_set<void **>> m_persistentObjects;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_GC_HEAP_H
