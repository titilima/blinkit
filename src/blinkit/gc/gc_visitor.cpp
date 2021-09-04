// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: gc_visitor.cpp
// Description: GCVistor Class
//      Author: Ziming Li
//     Created: 2020-11-15
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "./gc_visitor.h"

#include "blinkit/blink/renderer/wtf/MainThread.h"
#include "third_party/zed/include/zed/container_utilites.hpp"

namespace BlinKit {

#ifndef NDEBUG
static bool collectingGarbage = false;
#endif

GCVisitor::GCVisitor(GCObject *root)
{
    ASSERT(isMainThread());

#ifndef NDEBUG
    ASSERT(!collectingGarbage);
    collectingGarbage = true;
#endif

    if (nullptr != root)
    {
        ASSERT(0 == root->m_refCnt);
        m_objects.emplace(root, Slots());
    }
}

GCVisitor::~GCVisitor(void)
{
    ASSERT(collectingGarbage);

    std::vector<std::unique_ptr<GCObject>> objectsToGC;
    for (auto &[o, slots] : m_objects)
    {
        if (slots.size() < o->m_refCnt)
            continue; // Still retained by others.

        ASSERT(o->m_refCnt == slots.size());
        if (1 == o->m_refCnt)
            continue; // Leave it to the dtor.

        for (void **slot : slots)
            *slot = nullptr;
        objectsToGC.emplace_back(o);
    }

#ifndef NDEBUG
    collectingGarbage = false;
#endif
}

void GCVisitor::TraceImpl(GCObject *o, void **slot)
{
    ASSERT(isMainThread());
    ASSERT(nullptr != o);

    Slots &slots = m_objects[o];
    if (zed::key_exists(slots, slot))
        return;

    slots.emplace(slot);
    if (1 == slots.size())
        o->trace(this);
}

void GCVisitor::TraceObjectSet(GCObjectSetCallback &callback)
{
    ASSERT(false); // BKTODO:
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef NDEBUG
bool IsCollectingGarbage(void)
{
    return collectingGarbage;
}
#endif

} // namespace BlinKit
