#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: gc_session.h
// Description: GCSession Struct
//      Author: Ziming Li
//     Created: 2021-09-10
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_GC_SESSION_H
#define BLINKIT_BLINKIT_GC_SESSION_H

#include <unordered_set>
#include "blinkit/gc/gc_def.h"

namespace BlinKit {

struct GCSession
{
    using Slots = std::unordered_set<void **>;

    std::vector<GCObject *> RootMembers;
    std::unordered_map<GCObject *, Slots> MemberObjects;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_GC_SESSION_H
