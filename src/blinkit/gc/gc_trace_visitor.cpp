// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: gc_trace_visitor.cpp
// Description: GCTraceVisitor Class
//      Author: Ziming Li
//     Created: 2021-09-10
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./gc_trace_visitor.h"

#include "blinkit/blink/renderer/wtf/MainThread.h"
#include "blinkit/gc/gc_object_set.h"
#include "third_party/zed/include/zed/container_utilites.hpp"
#include "third_party/zed/include/zed/utility.hpp"

namespace BlinKit {

GCTraceVisitor::GCTraceVisitor(GCSession &session) : m_session(session)
{
}

void GCTraceVisitor::TraceImpl(GCRefPtrBase &ptr)
{
    ASSERT(isMainThread());

    GCObject *o = ptr.m_object;
    ASSERT(nullptr != o);

    GCSession::Slots &slots = m_session.MemberObjects[o];
    if (zed::key_exists(slots, &ptr))
        return;

    slots.emplace(&ptr);
    if (slots.size() > 1)
        return;

    o->trace(this);
}

void GCTraceVisitor::TraceObjectSet(GCObjectSetCallback &callback)
{
    callback.EnumObjects([](GCObject &o) {
        ASSERT(false); // BKTODO:
    });
}

} // namespace BlinKit
