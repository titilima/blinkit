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

#include "blinkit/gc/gc_def.h"

namespace BlinKit {

GCVisitor::GCVisitor(const std::unordered_set<void *> &memberObjects) : m_objectsToGC(memberObjects)
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
    GCObjectHeader::From(p)->gcPtr->Tracer(p, this);
}

} // namespace BlinKit
