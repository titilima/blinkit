// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: gc_cleanup_visitor.cpp
// Description: GCCleanupVisitor Class
//      Author: Ziming Li
//     Created: 2021-09-13
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./gc_cleanup_visitor.h"

#include "blinkit/gc/gc_object_set.h"
#include "third_party/zed/include/zed/container_utilites.hpp"

namespace BlinKit {

GCCleanupVisitor::GCCleanupVisitor(GCSession &session) : m_session(session)
{
}

GCCleanupVisitor::~GCCleanupVisitor(void)
{
    if (nullptr != m_session.RootMember)
    {
        if (IsFullyRetained(*m_session.RootMember))
            m_objectsToCleanup.emplace(m_session.RootMember);
    }

    std::vector<std::unique_ptr<GCObject>> objectsToCleanup;
    for (GCObject *o : m_objectsToCleanup)
    {
        auto it = m_session.MemberObjects.find(o);
        ASSERT(m_session.MemberObjects.end() != it);

        for (GCRefPtrBase *slot : it->second)
        {
            ASSERT(slot->m_object == o);
            slot->m_object = nullptr;
        }
        objectsToCleanup.emplace_back(o);

        m_session.MemberObjects.erase(it);
    }
}

bool GCCleanupVisitor::IsFullyRetained(const GCObject &o) const
{
    auto it = m_session.MemberObjects.find(const_cast<GCObject *>(&o));
    ASSERT(m_session.MemberObjects.end() != it);

    if (it->second.size() < o.m_refCnt)
        return false;

    ASSERT(it->second.size() == o.m_refCnt);
    return true;
}

void GCCleanupVisitor::TraceImpl(GCRefPtrBase &ptr)
{
    GCObject *o = ptr.m_object;
    if (!zed::key_exists(m_session.MemberObjects, o))
        return;

    if (zed::key_exists(m_tracedObjects, o))
        return;
    m_tracedObjects.emplace(o);

    if (IsFullyRetained(*o))
    {
        m_objectsToCleanup.emplace(o);
        o->trace(this);
    }
}

void GCCleanupVisitor::TraceObjectSet(GCObjectSetCallback &callback)
{
    callback.EnumObjects([](GCObject &o) {
        ASSERT(false); // BKTODO:
    });
}

} // namespace BlinKit
