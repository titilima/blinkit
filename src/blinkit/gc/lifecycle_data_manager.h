#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: lifecycle_data_manager.h
// Description: LifecycleDataManager Class
//      Author: Ziming Li
//     Created: 2021-08-19
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_LIFECYCLE_DATA_MANAGER_H
#define BLINKIT_BLINKIT_LIFECYCLE_DATA_MANAGER_H

#include <unordered_set>
#include "blinkit/gc/gc_def.h"

namespace BlinKit {

class LifecycleDataManager
{
public:
    LifecycleDataManager(void) = default;

    void ProcessObjectFinalizing(GCObject *o);

    void AttachWeakSlot(GCObject *o, void **slot);
    void DetachWeakSlot(GCObject *o, void **slot);

    void RegisterObserver(GCObject *o, GCLifecycleObserver *ob);
    void RemoveObserver(GCLifecycleObserver *ob);
private:
    struct DataEntry {
        std::unordered_set<void **> WeakSlots;
        std::unordered_set<GCLifecycleObserver *> Observers;
    };
    std::unordered_map<GCObject *, DataEntry> m_lifecycleData;
    std::unordered_map<GCLifecycleObserver *, std::unordered_set<GCObject *>> m_observers;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_LIFECYCLE_DATA_MANAGER_H
