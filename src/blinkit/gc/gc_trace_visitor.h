#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: gc_trace_visitor.h
// Description: GCTraceVisitor Class
//      Author: Ziming Li
//     Created: 2021-09-10
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_GC_TRACE_VISITOR_H
#define BLINKIT_BLINKIT_GC_TRACE_VISITOR_H

#include "blinkit/blink/renderer/platform/heap/Visitor.h"
#include "blinkit/gc/gc_session.h"

namespace BlinKit {

class GCTraceVisitor final : public blink::Visitor
{
public:
    GCTraceVisitor(GCSession &session);
private:
    void TraceImpl(GCRefPtrBase &ptr) override;
    void TraceObjectSet(GCObjectSetCallback &callback) override;

    GCSession &m_session;
    const bool m_fastTrace;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_GC_TRACE_VISITOR_H
