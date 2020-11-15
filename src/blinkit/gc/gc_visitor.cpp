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

namespace BlinKit {

GCVisitor::GCVisitor(void) : m_currentWorker(&GCVisitor::Collect)
{
}

void GCVisitor::Collect(void *p)
{
    ASSERT(false); // BKTODO:
}

void GCVisitor::Save(void *p)
{
    ASSERT(false); // BKTODO:
}

void GCVisitor::TraceImpl(void *p)
{
    if (nullptr != p)
        (this->*m_currentWorker)(p);
}

} // namespace BlinKit
