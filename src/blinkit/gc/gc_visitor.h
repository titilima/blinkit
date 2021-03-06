// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: gc_visitor.h
// Description: GCVisitor Class
//      Author: Ziming Li
//     Created: 2020-11-15
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_GC_VISITOR_H
#define BLINKIT_BLINKIT_GC_VISITOR_H

#pragma once

#include <unordered_set>
#include "third_party/blink/renderer/platform/heap/visitor.h"

namespace BlinKit {

class GCVisitor final : public blink::Visitor
{
public:
    GCVisitor(const std::unordered_set<void *> &memberObjects);

    const std::unordered_set<void *>& ObjectsToGC(void) const { return m_objectsToGC; }
    const std::vector<void **> WeakSlots(void) const { return m_weakSlots; }
private:
    void MainHandler(void *p);
    void ChildrenHandler(void *p);

    void TraceImpl(void *p) override;
    void RegisterWeakSlot(void **pp) override;

    std::unordered_set<void *> m_objectsToGC;
    std::vector<void **> m_weakSlots;
    std::vector<void *> m_childrenStash;

    using TraceHandler = void(GCVisitor::*)(void *);
    TraceHandler m_currentHandler = &GCVisitor::MainHandler;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_GC_VISITOR_H
