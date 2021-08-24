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

namespace BlinKit {

GCVisitor::GCVisitor(GCObject *root)
{
    ASSERT(isMainThread());
    if (nullptr != root)
    {
        ASSERT(0 == root->m_refCnt);
        m_objects.emplace(root, Slots());
    }
}

GCVisitor::~GCVisitor(void)
{
    std::vector<std::unique_ptr<GCObject>> objectsToGC;
    for (auto &[o, slots] : m_objects)
    {
        if (slots.size() < o->m_refCnt)
            continue; // Still retained by others.
        ASSERT(o->m_refCnt == slots.size());

        for (void **slot : slots)
            *slot = nullptr;
        objectsToGC.emplace_back(o);
    }
}

void GCVisitor::TraceImpl(GCObject *o, void **slot)
{
    ASSERT(isMainThread());
    ASSERT(nullptr != o);

    Slots &slots = m_objects[o];
    if (!slots.empty())
        ASSERT(slots.back() != slot);
    slots.emplace_back(slot);

    if (1 == slots.size())
        o->trace(this);
}

} // namespace BlinKit
