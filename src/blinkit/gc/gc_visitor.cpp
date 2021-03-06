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

#include "gc_visitor.h"

#include "base/auto_reset.h"
#include "blinkit/gc/gc_heap.h"

namespace BlinKit {

GCVisitor::GCVisitor(const std::unordered_set<void *> &memberObjects) : m_objectsToGC(memberObjects)
{
}

void GCVisitor::ChildrenHandler(void *p)
{
    m_childrenStash.push_back(p);
}

void GCVisitor::MainHandler(void *p)
{
    base::AutoReset<TraceHandler> handlerGuard(&m_currentHandler, &GCVisitor::ChildrenHandler);

    m_childrenStash.push_back(p);
    while (!m_childrenStash.empty())
    {
        std::vector<void *> childrenStash;
        childrenStash.swap(m_childrenStash);
        for (void *child : childrenStash)
            GCHeap::Trace(child, this);
    }
}

void GCVisitor::RegisterWeakSlot(void **pp)
{
    if (nullptr != *pp)
        m_weakSlots.push_back(pp);
}

void GCVisitor::TraceImpl(void *p)
{
    if (nullptr == p)
        return;

    auto it = m_objectsToGC.find(p);
    if (std::end(m_objectsToGC) == it)
        return;

    m_objectsToGC.erase(p);
    (this->*m_currentHandler)(p);
}

} // namespace BlinKit
