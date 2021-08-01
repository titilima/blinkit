#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: Heap.h
// Description: GC Classes
//      Author: Ziming Li
//     Created: 2021-06-29
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_HEAP_H
#define BLINKIT_BLINKIT_HEAP_H

#include "blinkit/blink/renderer/platform/heap/Visitor.h"
#include "blinkit/blink/renderer/wtf/Forward.h"
#include "blinkit/blink/renderer/wtf/HashSet.h"
#include "blinkit/blink/renderer/wtf/PassOwnPtr.h"
#include "blinkit/gc/gc_def.h"

namespace blink {

template <typename T>
class GarbageCollected
{
};

} // namespace blink

#define DECLARE_EAGER_FINALIZATION_OPERATOR_NEW()
#define WILL_NOT_BE_EAGERLY_TRACED_CLASS(TYPE)
#define EAGERLY_FINALIZE()
#define EAGERLY_FINALIZE_WILL_BE_REMOVED()

#endif // BLINKIT_BLINKIT_HEAP_H
