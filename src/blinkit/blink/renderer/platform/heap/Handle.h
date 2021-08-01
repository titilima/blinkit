#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: Handle.h
// Description: Placeholders for Heap Part
//      Author: Ziming Li
//     Created: 2021-06-29
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_HANDLE_H
#define BLINKIT_BLINKIT_HANDLE_H

#include "blinkit/blink/public/platform/WebTraceLocation.h"
#include "blinkit/blink/renderer/platform/heap/GarbageCollected.h"
#include "blinkit/blink/renderer/platform/heap/HeapAllocator.h"
#include "blinkit/blink/renderer/platform/heap/member.h"
#include "blinkit/blink/renderer/platform/heap/Visitor.h"
#include "blinkit/blink/renderer/wtf/MainThread.h"
#include "blinkit/blink/renderer/wtf/TypeTraits.h"

namespace blink {

template<typename T>
T* adoptPtrWillBeNoop(T *ptr)
{
    return ptr;
}

template<typename T>
T* adoptRefWillBeNoop(T *ptr)
{
    return ptr;
}

} // namespace blink

#define NoBaseWillBeGarbageCollected                blink::GarbageCollected
#define NoBaseWillBeGarbageCollectedFinalized       blink::GarbageCollectedFinalized
#define OwnPtrWillBeMember                          blink::Member
#define OwnPtrWillBePersistent                      blink::Persistent
#define OwnPtrWillBeRawPtr                          WTF::RawPtr
#define PassOwnPtrWillBeRawPtr                      WTF::RawPtr
#define PassRefPtrWillBeRawPtr                      WTF::RawPtr
#define PersistentHeapVectorWillBeHeapVector        blink::HeapVector
#define PersistentWillBeMember                      blink::Member
#define RawPtrWillBeMember                          blink::Member
#define RawPtrWillBeUntracedMember                  blink::UntracedMember
#define RawPtrWillBeWeakMember                      blink::WeakMember
#define RefCountedWillBeGarbageCollected            blink::GarbageCollected
#define RefCountedWillBeGarbageCollectedFinalized   blink::GarbageCollectedFinalized
#define RefCountedWillBeRefCountedGarbageCollected  blink::RefCountedGarbageCollected
#define RefPtrWillBeMember                          blink::Member
#define RefPtrWillBePersistent                      blink::Persistent
#define RefPtrWillBeRawPtr                          WTF::RawPtr
#define WeakPtrWillBeMember                         blink::Member
#define WeakPtrWillBeRawPtr                         WTF::RawPtr
#define WeakPtrWillBeWeakMember                     blink::WeakMember
#define WillBeGarbageCollectedMixin                 blink::GarbageCollectedMixin
#define WillBeHeapDeque                             blink::HeapDeque
#define WillBeHeapHashMap                           blink::HeapHashMap
#define WillBeHeapHashSet                           blink::HeapHashSet
#define WillBeHeapLinkedHashSet                     blink::HeapLinkedHashSet
#define WillBeHeapLinkedStack                       blink::HeapLinkedStack
#define WillBeHeapListHashSet                       blink::HeapListHashSet
#define WillBeHeapSupplement                        blink::HeapSupplement
#define WillBeHeapSupplementable                    blink::HeapSupplementable
#define WillBeHeapTerminatedArray                   blink::HeapTerminatedArray
#define WillBeHeapVector                            blink::HeapVector

#define LEAK_SANITIZER_DISABLED_SCOPE

#define USING_FAST_MALLOC_WILL_BE_REMOVED(type)
#define USING_FAST_MALLOC_WITH_TYPE_NAME_WILL_BE_REMOVED(type)

#define DECLARE_EMPTY_DESTRUCTOR_WILL_BE_REMOVED(type)
#define DEFINE_EMPTY_DESTRUCTOR_WILL_BE_REMOVED(type)
#define DECLARE_EMPTY_VIRTUAL_DESTRUCTOR_WILL_BE_REMOVED(type)

// BKTODO: Check the original logics
#define USING_PRE_FINALIZER(Class, preFinalizer)
#define WILL_BE_USING_PRE_FINALIZER(Class, method)

#endif // BLINKIT_BLINKIT_HANDLE_H
