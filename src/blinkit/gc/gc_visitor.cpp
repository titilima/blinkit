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

#include "blinkit/gc/gc_heap.h"

namespace BlinKit {

GCVisitor::GCVisitor(std::unordered_set<void *> &objectsToGC) : m_objectsToGC(objectsToGC)
{
}

void GCVisitor::TraceImpl(void *p)
{
    if (nullptr == p)
        return;

    auto it = m_objectsToGC.find(p);
    if (std::end(m_objectsToGC) == it)
        return;

    m_objectsToGC.erase(p);
    GCHeap::Trace(p, this);
}

} // namespace BlinKit
