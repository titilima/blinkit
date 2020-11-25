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

template <typename T>
class GarbageCollected;
class ScriptWrappable;

class Visitor
{
public:
    template <class T, typename = std::enable_if<std::is_base_of<ScriptWrappable, T>::value>>
    inline void Trace(const T *po)
    {
        TraceImpl(const_cast<T *>(po));
    }
    template <typename T>
    inline void Trace(const Member<T> &m)
    {
        const void *pm = m.Get();
        TraceImpl(const_cast<void *>(pm));
    }
    template <typename T, typename K, typename H>
    void Trace(const std::unordered_map<K, T, H> &m)
    {
        for (auto &it : m)
            Trace(it.second);
    }
    template <typename T>
    void Trace(const std::vector<T> &v)
    {
        for (const T &o : v)
            Trace(o);
    }

    template <typename T> void Trace(T &o);
protected:
    Visitor(void) = default;

    virtual void TraceImpl(void *p) = 0;
};

} // namespace blink

#endif // BLINKIT_BLINK_VISITOR_H
