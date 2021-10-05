#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: gc_cleanup_visitor.h
// Description: GCCleanupVisitor Class
//      Author: Ziming Li
//     Created: 2021-09-13
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_GC_CLEANUP_VISITOR_H
#define BLINKIT_BLINKIT_GC_CLEANUP_VISITOR_H

#include "blinkit/blink/renderer/platform/heap/Visitor.h"
#include "blinkit/blink/renderer/wtf/Allocator.h"
#include "blinkit/gc/gc_session.h"

namespace BlinKit {

class GCCleanupVisitor final : public blink::Visitor
{
    STACK_ALLOCATED();
public:
    GCCleanupVisitor(GCSession &session);
    ~GCCleanupVisitor(void);
private:
    bool IsFullyRetained(const GCObject &o) const;

    void TraceImpl(GCRefPtrBase &ptr) override;
    void TraceObjectSet(GCObjectSetCallback &callback) override;

    GCSession &m_session;
    std::unordered_set<GCObject *> m_tracedObjects, m_objectsToCleanup;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_GC_CLEANUP_VISITOR_H
