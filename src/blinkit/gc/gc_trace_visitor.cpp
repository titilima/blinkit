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

static bool PerformFastTrace(const GCSession &session)
{
    if (nullptr == session.RootMember)
        return false;
    return GCObject::TreeNode != session.RootMember->GCCategory();
}

GCTraceVisitor::GCTraceVisitor(GCSession &session) : m_session(session), m_fastTrace(PerformFastTrace(session))
{
}

void GCTraceVisitor::TraceImpl(GCObject *o, void **slot)
{
    ASSERT(isMainThread());
    ASSERT(nullptr != o);

    if (m_fastTrace && o->IsRetainedInTree())
        return;

    GCSession::Slots &slots = m_session.MemberObjects[o];
    if (zed::key_exists(slots, slot))
        return;

    slots.emplace(slot);
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
