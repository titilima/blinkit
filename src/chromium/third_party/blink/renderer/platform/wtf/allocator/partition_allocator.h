// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: partition_allocator.h
// Description: Placeholders for WTF Part
//      Author: Ziming Li
//     Created: 2019-09-29
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_PARTITION_ALLOCATOR_H
#define BLINKIT_BLINK_PARTITION_ALLOCATOR_H

#pragma once

#include "third_party/blink/renderer/platform/wtf/allocator.h"

namespace WTF {

class PartitionAllocator
{
public:
    static constexpr bool kIsGarbageCollected = false;

    template <typename T>
    static size_t QuantizedSize(size_t count)
    {
        CHECK_LE(count, MaxElementCountInBackingStore<T>());
        return count * sizeof(T);
    }
    template <typename T>
    static size_t MaxElementCountInBackingStore(void)
    {
        return kGenericMaxDirectMapped / sizeof(T);
    }

    template <typename Return, typename Metadata>
    static Return Malloc(size_t size, const char* type_name)
    {
        return reinterpret_cast<Return>(WTF::Partitions::FastMalloc(size, type_name));
    }
    template <typename T>
    static T* AllocateInlineVectorBacking(size_t size)
    {
        return AllocateVectorBacking<T>(size);
    }
    template <typename T>
    static T* AllocateVectorBacking(size_t size)
    {
        return reinterpret_cast<T*>(AllocateBacking(size));
    }
    template <typename T>
    static T* AllocateExpandedVectorBacking(size_t size)
    {
        return reinterpret_cast<T*>(AllocateBacking(size));
    }
    template <typename T, typename HashTable>
    static T* AllocateHashTableBacking(size_t size)
    {
        return reinterpret_cast<T *>(AllocateBacking(size));
    }
    template <typename T, typename HashTable>
    static T* AllocateZeroedHashTableBacking(size_t size)
    {
        void* result = AllocateBacking(size);
        memset(result, 0, size);
        return reinterpret_cast<T *>(result);
    }
    static inline void FreeInlineVectorBacking(void *address)
    {
        FreeVectorBacking(address);
    }
    static void FreeVectorBacking(void *address)
    {
        free(address);
    }
    static void FreeHashTableBacking(void *address)
    {
        free(address);
    }
    static void Free(void *address)
    {
        WTF::Partitions::FastFree(address);
    }

    static inline bool ExpandVectorBacking(void *, size_t) { return false; }
    static inline bool ExpandHashTableBacking(void *, size_t) { return false; }
    static bool IsAllocationAllowed(void) { return true; }
    static bool IsObjectResurrectionForbidden(void) { return false; }
    static bool IsSweepForbidden(void) { return false; }

    static void EnterGCForbiddenScope(void) {}
    static void LeaveGCForbiddenScope(void) {}

    static void BackingWriteBarrier(void *) {}
    template <typename T, typename Traits> static void NotifyNewObject(T *object) {}
    template <typename T, typename Traits> static void NotifyNewObjects(T *array, size_t len) {}
    static inline bool ShrinkVectorBacking(void *address, size_t quantizedCurrentSize, size_t quantizedShrunkSize)
    {
        // Optimization: if we're downsizing inside the same allocator bucket,
        // we can skip reallocation.
        return quantizedCurrentSize == quantizedShrunkSize;
    }
    static inline bool ShrinkInlineVectorBacking(void *address, size_t quantizedCurrentSize, size_t quantizedShrunkSize)
    {
        return ShrinkVectorBacking(address, quantizedCurrentSize, quantizedShrunkSize);
    }
    static void TraceMarkedBackingStore(void *) {}
private:
    static void* AllocateBacking(size_t size)
    {
        return malloc(size);
    }

    static const size_t kGenericMaxDirectMapped = 0x7fffffff;
};

} // namespace WTF

#define USE_ALLOCATOR(ClassName, Allocator)                       \
 public:                                                          \
  void* operator new(size_t size) {                               \
    return Allocator::template Malloc<void*, ClassName>(          \
        size, WTF_HEAP_PROFILER_TYPE_NAME(ClassName));            \
  }                                                               \
  void operator delete(void* p) { Allocator::Free(p); }           \
  void* operator new[](size_t size) {                             \
    return Allocator::template NewArray<ClassName>(size);         \
  }                                                               \
  void operator delete[](void* p) { Allocator::DeleteArray(p); }  \
  void* operator new(size_t, NotNullTag, void* location) {        \
    DCHECK(location);                                             \
    return location;                                              \
  }                                                               \
  void* operator new(size_t, void* location) { return location; } \
                                                                  \
 private:                                                         \
  typedef int __thisIsHereToForceASemicolonAfterThisMacro

#endif  // BLINKIT_BLINK_PARTITION_ALLOCATOR_H
