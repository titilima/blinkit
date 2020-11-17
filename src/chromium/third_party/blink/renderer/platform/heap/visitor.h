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
#include "third_party/blink/renderer/platform/heap/garbage_collected.h"
#include "third_party/blink/renderer/platform/heap/member.h"

namespace blink {

class Visitor
{
public:
    template <typename T>
    inline void Trace(Member<T> &o)
    {
        TraceImpl(o.Get());
    }
    template <typename T>
    inline void Trace(GarbageCollected<T> *po)
    {
        TraceImpl(po);
    }
    template <typename T, typename K, typename H>
    void Trace(const std::unordered_map<K, GarbageCollected<T> *, H> &m)
    {
        for (auto &it : m)
            TraceImpl(it.second);
    }
    template <typename T>
    void Trace(const std::vector<GarbageCollected<T> *> &v)
    {
        for (GarbageCollected<T> *po : v)
            TraceImpl(po);
    }

    template <typename T>
    inline void Trace(const T &)
    {
        NOTREACHED();
    }
protected:
    Visitor(void) = default;

    virtual void TraceImpl(void *p) = 0;
};

} // namespace blink

#endif // BLINKIT_BLINK_VISITOR_H
