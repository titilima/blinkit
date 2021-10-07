#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: gc_listed_set.h
// Description: GCListedSet Class
//      Author: Ziming Li
//     Created: 2021-10-07
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_GC_LISTED_SET_H
#define BLINKIT_BLINKIT_GC_LISTED_SET_H

#include <list>
#include "blinkit/gc/gc_def.h"
#include "blinkit/blink/renderer/platform/heap/Visitor.h"
#include "third_party/zed/include/zed/container_utilites.hpp"

namespace BlinKit {

template <class T>
class GCListedSet : private std::list<GCRefPtr<T>>
{
    using BaseClass = std::list<GCRefPtr<T>>;
public:
    using BaseClass::begin;
    using BaseClass::empty;
    using BaseClass::end;

    void clear(void)
    {
        m_map.clear();
        BaseClass::clear();
    }
    bool contains(T *p) const { return zed::key_exists(m_map, p); }
    void erase(typename BaseClass::const_iterator it)
    {
        m_map.erase(it->get());
        BaseClass::erase(it);
    }
    void insert(T *p)
    {
        auto it = BaseClass::emplace(BaseClass::end(), p);
        m_map.emplace(p, it);
    }
    void trace(blink::Visitor *visitor)
    {
        auto it = BaseClass::begin();
        while (BaseClass::end() != it)
        {
            visitor->trace(*it);
            ++it;
        }
    }
private:
    std::unordered_map<T *, typename BaseClass::const_iterator> m_map;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_GC_LISTED_SET_H
