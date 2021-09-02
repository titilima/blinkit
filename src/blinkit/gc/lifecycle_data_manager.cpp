// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: lifecycle_data_manager.cpp
// Description: LifecycleDataManager Class
//      Author: Ziming Li
//     Created: 2021-08-19
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./lifecycle_data_manager.h"

#include "third_party/zed/include/zed/container_utilites.hpp"

namespace BlinKit {

void LifecycleDataManager::AttachWeakSlot(GCObject *o, void **slot)
{
    m_lifecycleData[o].WeakSlots.emplace(slot);
}

void LifecycleDataManager::DetachWeakSlot(GCObject *o, void **slot)
{
    ASSERT(zed::key_exists(m_lifecycleData, o));
    ASSERT(zed::key_exists(m_lifecycleData[o].WeakSlots, slot));
    m_lifecycleData[o].WeakSlots.erase(slot);
}

void LifecycleDataManager::ProcessObjectFinalizing(GCObject *o)
{
    auto it = m_lifecycleData.find(o);
    if (m_lifecycleData.end() == it)
        return;

    if (!it->second.Observers.empty())
    {
        for (GCLifecycleObserver *ob : it->second.Observers)
        {
            ob->ObjectFinalized(o);

            ASSERT(zed::key_exists(m_observers, ob));
            ASSERT(zed::key_exists(m_observers[ob], o));
            m_observers[ob].erase(o);
        }
    }

    for (void **slot : it->second.WeakSlots)
        *slot = nullptr;
    m_lifecycleData.erase(it);
}

void LifecycleDataManager::RegisterObserver(GCObject *o, GCLifecycleObserver *ob)
{
    m_lifecycleData[o].Observers.emplace(ob);
    m_observers[ob].emplace(o);
}

void LifecycleDataManager::RemoveObserver(GCLifecycleObserver *ob)
{
    ASSERT(zed::key_exists(m_observers, ob));
    for (GCObject *o : m_observers[ob])
        m_lifecycleData[o].Observers.erase(ob);
    m_observers.erase(ob);
}

void LifecycleDataManager::RemoveObserverForObject(GCObject *o, GCLifecycleObserver *ob)
{
    ASSERT(zed::key_exists(m_observers, ob));
    ASSERT(zed::key_exists(m_observers[ob], o));
    m_observers[ob].erase(o);

    ASSERT(zed::key_exists(m_lifecycleData, o));
    ASSERT(zed::key_exists(m_lifecycleData[o].Observers, ob));
    m_lifecycleData[o].Observers.erase(ob);
}

} // namespace BlinKit
