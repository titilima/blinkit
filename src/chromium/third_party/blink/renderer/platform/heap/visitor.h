// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: visitor.h
// Description: Placeholders for Heap Part
//      Author: Ziming Li
//     Created: 2020-08-29
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_VISITOR_H
#define BLINKIT_BLINK_VISITOR_H

#pragma once

#include <unordered_map>
#include <vector>
#include "third_party/blink/renderer/platform/heap/member.h"

namespace BlinKit {
template <typename T>
struct TracePolicy;
}

namespace blink {

template <typename T>
class GarbageCollected;
class ScriptWrappable;

class Visitor
{
    template <typename T> friend struct BlinKit::TracePolicy;
public:
    template <typename T>
    void Trace(T *po)
    {
        if (nullptr != po)
            TraceImpl(po);
    }

    template <typename T>
    void Trace(blink::Member<T> &m)
    {
        if (T *p = m.Get())
            TraceImpl(trace_cast<void *>(p));
    }
    template <typename T>
    void Trace(const blink::Member<T> &m)
    {
        if (T *p = m.Get())
            TraceImpl(trace_cast<void *>(p));
    }

    template <typename T>
    void Trace(blink::WeakMember<T> &m)
    {
        if (m)
            RegisterWeakSlot(trace_cast<void **>(&m.m_rawPtr));
    }
    template <typename T>
    void Trace(const blink::WeakMember<T> &m)
    {
        if (m)
            RegisterWeakSlot(trace_cast<void **>(&m.m_rawPtr));
    }

    template <typename T>
    void Trace(T &o)
    {
        BlinKit::TracePolicy<T>::Impl(o, this);
    }
protected:
    Visitor(void) = default;

    template <typename To, typename From>
    static To trace_cast(From from)
    {
        union {
            From f;
            To t;
        } u;
        u.f = from;
        return u.t;
    }

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
        o.Trace(visitor);
    }
};

template <typename T1, typename T2>
struct TracePolicy<std::pair<T1, T2>>
{
    static void Impl(std::pair<T1, T2> &p, blink::Visitor *visitor)
    {
        visitor->Trace(p.first);
        visitor->Trace(p.second);
    }
};

template <typename T, typename K, typename H, typename E>
struct TracePolicy<std::unordered_map<K, T, H, E>>
{
    static void Impl(std::unordered_map<K, T, H, E> &m, blink::Visitor *visitor)
    {
        for (auto &it : m)
            visitor->Trace(it.second);
    }
};

template <typename T>
struct TracePolicy<std::vector<T>>
{
    static void Impl(std::vector<T> &v, blink::Visitor *visitor)
    {
        for (auto &o : v)
            visitor->Trace(o);
    }
};

} // namespace BlinKit

#endif // BLINKIT_BLINK_VISITOR_H
