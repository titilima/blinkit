// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: allocator.h
// Description: Placeholders for WTF Part
//      Author: Ziming Li
//     Created: 2019-09-16
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_ALLOCATOR_H
#define BLINKIT_BLINK_ALLOCATOR_H

#pragma once

#include "third_party/blink/renderer/platform/wtf/allocator/partitions.h"

#define DISALLOW_NEW()                          \
    void* operator new(size_t) = delete;        \
    void* operator new(size_t, void *) = delete

#define STACK_ALLOCATED() DISALLOW_NEW()

#define STATIC_ONLY(Type)                       \
    Type(void) = delete;                        \
    Type(const Type &) = delete;                \
    Type& operator=(const Type &) = delete;     \
    void* operator new(size_t) = delete;        \
    void* operator new(size_t, void *) = delete

#define USING_FAST_MALLOC_INTERNAL(type, typeName)                      \
public:                                                                 \
    void* operator new(size_t, void* p) { return p; }                   \
    void* operator new[](size_t, void* p) { return p; }                 \
    void* operator new(size_t size) {                                   \
        return ::WTF::Partitions::FastMalloc(size, typeName);           \
    }                                                                   \
    void operator delete(void* p) { ::WTF::Partitions::FastFree(p); }   \
    void* operator new[](size_t size) {                                 \
        return ::WTF::Partitions::FastMalloc(size, typeName);           \
    }                                                                   \
    void operator delete[](void* p) { ::WTF::Partitions::FastFree(p); } \
    void* operator new(size_t, NotNullTag, void* location) {            \
        DCHECK(location);                                               \
        return location;                                                \
    }

#define WTF_HEAP_PROFILER_TYPE_NAME(T)  nullptr

#define USING_FAST_MALLOC(type) USING_FAST_MALLOC_INTERNAL(type, WTF_HEAP_PROFILER_TYPE_NAME(type))

enum NotNullTag { NotNull };
inline void* operator new(size_t, NotNullTag, void *location)
{
    DCHECK(location);
    return location;
}

#endif // BLINKIT_BLINK_ALLOCATOR_H
