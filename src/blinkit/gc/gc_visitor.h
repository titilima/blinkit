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
    GCVisitor(void);

    void SwitchToSaveMode(void) { m_currentWorker = &GCVisitor::Save; }
private:
    void Collect(void *p);
    void Save(void *p);
    void TraceImpl(void *p) override;

    using TraceWorker = void (GCVisitor::*)(void*);

    std::unordered_set<void *> m_objects;
    TraceWorker m_currentWorker;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_GC_VISITOR_H
