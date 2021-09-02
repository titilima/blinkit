// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: gc_root_object.cpp
// Description: GCRootObject Class
//      Author: Ziming Li
//     Created: 2021-08-24
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./gc_def.h"

#include "blinkit/gc/gc_visitor.h"

using namespace blink;

namespace BlinKit {

void GCRootObject::CollectGarbage(const std::function<void(Visitor *)> &callback)
{
    GCVisitor visitor;
    callback(&visitor);
    m_garbageCollected = true;
}

} // namespace BlinKit
