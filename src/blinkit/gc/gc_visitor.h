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

#include "blinkit/blink/renderer/platform/heap/Visitor.h"
#include "blinkit/blink/renderer/wtf/Allocator.h"

namespace BlinKit {

class GCVisitor final : public blink::Visitor
{
    STACK_ALLOCATED();
public:
    GCVisitor(GCObject *root = nullptr);
    ~GCVisitor(void);
private:
    void TraceImpl(GCObject *o, void **slot) override;

    using Slots = std::vector<void **>;
    std::unordered_map<GCObject *, Slots> m_objects;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_GC_VISITOR_H
