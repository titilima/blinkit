// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: garbage_collector.cpp
// Description: GarbageCollector Class
//      Author: Ziming Li
//     Created: 2021-09-10
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./garbage_collector.h"

#include "blinkit/blink/renderer/wtf/MainThread.h"
#include "blinkit/gc/gc_cleanup_visitor.h"
#include "blinkit/gc/gc_trace_visitor.h"
#include "third_party/zed/include/zed/container_utilites.hpp"

using namespace blink;

namespace BlinKit {

#ifndef NDEBUG
static bool collectingGarbage = false;
#endif

GarbageCollector::GarbageCollector(GCObject &o) : m_tracer(std::bind(&GCObject::trace, &o, std::placeholders::_1))
{
    Initialize();
    m_session->RootMember = &o;
    m_session->MemberObjects.emplace(&o, GCSession::Slots());
}

GarbageCollector::GarbageCollector(const Tracer &tracer) : m_tracer(tracer)
{
    Initialize();
}

GarbageCollector::~GarbageCollector(void)
{
    GCCleanupVisitor visitor(*m_session);
    m_tracer(&visitor);

#ifndef NDEBUG
    ASSERT(collectingGarbage);
    collectingGarbage = false;
#endif
}

void GarbageCollector::Initialize(void)
{
    ASSERT(isMainThread());

#ifndef NDEBUG
    ASSERT(!collectingGarbage);
    collectingGarbage = true;
#endif

    m_session = std::make_unique<GCSession>();
}

void GarbageCollector::Perform(void)
{
    GCTraceVisitor visitor(*m_session);
    m_tracer(&visitor);
}

void GarbageCollector::PerformOnMember(GCObject &o)
{
    GarbageCollector gc(o);
    gc.Perform();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef NDEBUG
bool IsCollectingGarbage(void)
{
    return collectingGarbage;
}
#endif

} // namespace BlinKit
