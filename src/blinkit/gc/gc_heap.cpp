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
#include "blinkit/gc/gc_visitor.h"
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
    CleanupPersistentObjects();

    theHeap = nullptr;
}

void GCHeap::CleanupGlobalObjects(void)
{
    auto it = m_globalObjects.begin();
    while (m_globalObjects.end() != it)
    {
        GCObject *o = *it;

        it = m_globalObjects.erase(it);
        o->DecRef();
        ASSERT(0 == o->m_refCnt);

        GCVisitor visitor(o);
        o->trace(&visitor);
    }
}

void GCHeap::CleanupPersistentObjects(void)
{
    auto it = m_persistentObjects.begin();
    while (m_persistentObjects.end() != it)
    {
        GCObject *o = it->first;
        auto &slots = it->second;

        ASSERT(slots.size() == o->m_refCnt);
        for (void **slot : slots)
            *slot = nullptr;
        o->m_refCnt -= slots.size();

        it = m_persistentObjects.erase(it);

        GCVisitor visitor(o);
        o->trace(&visitor);
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

void GCHeap::ReleasePersistentObject(GCObject &o, void **slot)
{
    ASSERT(zed::key_exists(m_persistentObjects, &o));

    auto &slots = m_persistentObjects[&o];
    ASSERT(zed::key_exists(slots, slot));
    ASSERT(slots.size() <= o.m_refCnt);

    slots.erase(slot);
    if (slots.empty())
        m_persistentObjects.erase(&o);
    *slot = nullptr;

    if (o.DecRef() > 0)
        return;

    GCVisitor visitor(&o);
    o.trace(&visitor);
}

void GCHeap::RemoveLifecycleObserver(GCLifecycleObserver *ob)
{
    theHeap->m_lifecycleDataManager.RemoveObserver(ob);
}

void GCHeap::RemoveLifecycleObserverForObject(GCObject *o, GCLifecycleObserver *ob)
{
    theHeap->m_lifecycleDataManager.RemoveObserverForObject(o, ob);
}

void GCHeap::RetainPersistentObject(GCObject &o, void **slot)
{
    ASSERT(!zed::key_exists(m_persistentObjects, &o) || !zed::key_exists(m_persistentObjects[&o], slot));

    auto &slots = m_persistentObjects[&o];
    slots.emplace(slot);
    o.IncRef();

    ASSERT(slots.size() <= o.m_refCnt);
}

void GCHeap::SetGlobalObject(GCObject &o)
{
    ASSERT(!zed::key_exists(m_globalObjects, &o));
    o.IncRef();
    m_globalObjects.emplace(&o);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

void GCReleasePersistentObject(GCObject *o, void **slot)
{
    if (nullptr != o && nullptr != theHeap)
        theHeap->ReleasePersistentObject(*o, slot);
}

void GCRetainPersistentObject(GCObject *o, void **slot)
{
    if (nullptr != o)
        theHeap->RetainPersistentObject(*o, slot);
}

void GCSetGlobalObject(GCObject *o)
{
    if (nullptr != o)
        theHeap->SetGlobalObject(*o);
}

} // namespace BlinKit
