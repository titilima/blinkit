#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: Visitor.h
// Description: Placeholders for Heap Part
//      Author: Ziming Li
//     Created: 2021-06-29
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_VISITOR_H
#define BLINKIT_BLINKIT_VISITOR_H

#include <unordered_set>
#include "blinkit/blink/renderer/platform/heap/member.h"
#include "blinkit/gc/gc_def.h"
#include "third_party/zed/include/zed/memory.hpp"

namespace BlinKit {
template <typename T>
struct TracePolicy;
}

namespace blink {

template <typename T>
class GarbageCollected;

class Visitor
{
    template <typename T> friend struct BlinKit::TracePolicy;
public:
    template <typename T>
    void trace(T *po)
    {
        if (nullptr != po)
            TraceImpl(po);
    }
    template <typename T>
    void trace(BlinKit::GCMember<T> &m)
    {
        if (m)
            TraceImpl(*m);
    }

    template <typename T>
    void trace(blink::Member<T> &m)
    {
        if (T *p = m.get())
            TraceImpl(zed::bit_cast<void *>(p));
    }
    template <typename T>
    void trace(const blink::Member<T> &m)
    {
        if (T *p = m.get())
            TraceImpl(zed::bit_cast<void *>(p));
    }

    template <typename T>
    void trace(blink::WeakMember<T> &m)
    {
        if (m)
            RegisterWeakSlot(zed::bit_cast<void **>(&m.m_rawPtr));
    }
    template <typename T>
    void trace(const blink::WeakMember<T> &m)
    {
        if (m)
            RegisterWeakSlot(zed::bit_cast<void **>(&m.m_rawPtr));
    }

    template <typename T>
    void trace(T &o)
    {
        BlinKit::TracePolicy<T>::Impl(o, this);
    }
protected:
    Visitor(void) = default;

    virtual void TraceImpl(BlinKit::GCObject &o) = 0;
    virtual void TraceImpl(void *p) = 0;
    virtual void RegisterWeakSlot(void **slot) = 0;
};

} // namespace blink

namespace BlinKit {

template <typename T>
struct TracePolicy
{
    static void Impl(T &o, blink::Visitor *visitor)
    {
        o.trace(visitor);
    }
};

template <typename T, typename K, typename H>
struct TracePolicy<std::unordered_map<K, T, H>>
{
    static void Impl(std::unordered_map<K, T, H> &m, blink::Visitor *visitor)
    {
        for (auto &[_, v] : m)
            visitor->trace(v);
    }
};

template <typename T>
struct TracePolicy<std::unordered_set<T>>
{
    static void Impl(std::unordered_set<T> &s, blink::Visitor *visitor)
    {
        for (auto &v : s)
            visitor->trace(v);
    }
};

template <typename T>
struct TracePolicy<std::vector<T>>
{
    static void Impl(std::vector<T> &v, blink::Visitor *visitor)
    {
        for (auto &o : v)
            visitor->trace(o);
    }
};

} // namespace BlinKit

#define DECLARE_TRACE_AFTER_DISPATCH()  \
public:                                 \
    void traceAfterDispatch(Visitor *)

#define DEFINE_TRACE_AFTER_DISPATCH(T)              \
    void T::traceAfterDispatch(Visitor *visitor)

#define DEFINE_INLINE_TRACE_AFTER_DISPATCH()    \
    void traceAfterDispatch(Visitor *visitor)

#define DECLARE_TRACE_IMPL(MaybeVirtual)                                     \
public:                                                                      \
    MaybeVirtual void trace(Visitor *);

#define DEFINE_INLINE_TRACE_IMPL(MaybeVirtual)                               \
public:                                                                      \
    MaybeVirtual void trace(Visitor *visitor)

#define EMPTY_MACRO_ARGUMENT

#define DECLARE_TRACE()                 DECLARE_TRACE_IMPL(EMPTY_MACRO_ARGUMENT)
#define DECLARE_VIRTUAL_TRACE()         DECLARE_TRACE_IMPL(virtual)
#define DEFINE_INLINE_TRACE()           DEFINE_INLINE_TRACE_IMPL(EMPTY_MACRO_ARGUMENT)
#define DEFINE_INLINE_VIRTUAL_TRACE()   DEFINE_INLINE_TRACE_IMPL(virtual)

#define DEFINE_TRACE(T)             \
    void T::trace(Visitor *visitor)

#endif // BLINKIT_BLINKIT_VISITOR_H
