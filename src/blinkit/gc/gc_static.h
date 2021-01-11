// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: gc_static.h
// Description: GCStatic Class
//      Author: Ziming Li
//     Created: 2020-12-14
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_GC_STATIC_H
#define BLINKIT_BLINKIT_GC_STATIC_H

#pragma once

#include <utility>
#include "third_party/blink/renderer/platform/heap/garbage_collected.h"

namespace BlinKit {

template <typename T>
struct GCEmptyTracer
{
    static void Impl(T &, blink::Visitor *) {}
};

template <typename T, typename TracePolicy = GCEmptyTracer<T>>
class GCStaticWrapper
{
public:
    template <typename... Args>
    GCStaticWrapper(Args&&... args) : m_ptr(Alloc(std::forward<Args>(args)...))
    {
    }

    T* GetAsPointer(void) { return m_ptr; }
    T& GetAsReference(void) { return *m_ptr; }
private:
    template <typename... Args>
    static T* Alloc(Args&&... args)
    {
        void *p = GCHeapAlloc(GCObjectType::Global, sizeof(T), GCPtr()
#ifndef NDEBUG
            , "GCStaticObject"
#endif
        );
        return new (p) T(std::forward<Args>(args)...);
    }
    static GCTable* GCPtr(void)
    {
        static GCTable s_gcTable = { Deleter, Tracer };
        return &s_gcTable;
    }
    static void Deleter(void *ptr) { reinterpret_cast<T *>(ptr)->~T(); }
    static void Tracer(void *ptr, blink::Visitor *visitor)
    {
        TracePolicy::Impl(*(reinterpret_cast<T *>(ptr)), visitor);
    }

    T *m_ptr;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_GC_STATIC_H
