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
template <typename T> struct TracePolicy {};
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
        TraceImpl(po);
    }

    template <typename T>
    void Trace(blink::Member<T> &m)
    {
        TraceImpl(m.Get());
    }
    template <typename T>
    void Trace(const blink::Member<T> &m)
    {
        TraceImpl(m.Get());
    }

    template <typename T>
    void Trace(T &o)
    {
        BlinKit::TracePolicy<T>::Impl(o, this);
    }
protected:
    Visitor(void) = default;

    virtual void TraceImpl(void *p) = 0;
};

} // namespace blink

namespace BlinKit {

template <typename T, typename K, typename H>
struct TracePolicy<std::unordered_map<K, T, H>>
{
    static void Impl(std::unordered_map<K, T, H> &m, blink::Visitor *visitor)
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
