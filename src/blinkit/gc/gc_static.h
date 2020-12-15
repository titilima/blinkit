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

#include "third_party/blink/renderer/platform/heap/garbage_collected.h"

namespace BlinKit {

template <typename T>
class GCStaticWrapper
{
public:
    GCStaticWrapper(const T &other)
    {
        void *p = GCHeapAlloc(GCObjectType::Global, sizeof(T), GCPtr()
#ifndef NDEBUG
            , "GCStaticObject"
#endif
        );
        m_ptr = new (p) T(other);
    }

    T* GetAsPointer(void) { return m_ptr; }
    T& GetAsReference(void) { return *m_ptr; }
private:
    static GCTable* GCPtr(void)
    {
        static GCTable s_gcTable = { Deleter, Tracer };
        return &s_gcTable;
    }
    static void Deleter(void *ptr) { reinterpret_cast<T *>(ptr)->~T(); }
    static void Tracer(void *, blink::Visitor *) {}

    T *m_ptr;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_GC_STATIC_H
