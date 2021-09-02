#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: gc_weak_object_set.h
// Description: GCWeakObjectSet Class
//      Author: Ziming Li
//     Created: 2021-08-29
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_GC_WEAK_OBJECT_SET_H
#define BLINKIT_BLINKIT_GC_WEAK_OBJECT_SET_H

#include <unordered_set>
#include "blinkit/gc/gc_def.h"
#include "third_party/zed/include/zed/container_utilites.hpp"

namespace BlinKit {

class GCWeakObjectSetImpl : public GCLifecycleObserver, protected std::unordered_set<GCObject *>
{
    using StdSet = std::unordered_set<GCObject *>;
public:
    using StdSet::empty;
protected:
    bool contains(GCObject *o) const
    {
        return zed::key_exists(static_cast<const StdSet &>(*this), o);
    }
    void emplace(GCObject *o)
    {
        StdSet::emplace(o);
        GCLifecycleObserver::Observe(o);
    }
    void erase(GCObject *o)
    {
        GCLifecycleObserver::Unobserve(o);
        StdSet::erase(o);
    }
    StdSet::iterator erase(StdSet::iterator it)
    {
        GCLifecycleObserver::Unobserve(*it);
        return StdSet::erase(it);
    }
private:
    void ObjectFinalized(GCObject *o) final
    {
        StdSet::erase(o);
    }
};

template <class T>
class GCWeakObjectSet final : public GCWeakObjectSetImpl
{
    using StdSet = std::unordered_set<GCObject *>;
public:
    class iterator
    {
    public:
        iterator(typename StdSet::iterator it) : m_it(it) {}

        bool operator==(const iterator &o) const { return m_it == o.m_it; }
        bool operator!=(const iterator &o) const { return m_it != o.m_it; }

        iterator& operator++(void)
        {
            ++m_it;
            return *this;
        }

        T* operator->() const { return static_cast<T *>(*m_it); }
        T& operator*() const { return *static_cast<T *>(*m_it); }
    private:
        friend class GCWeakObjectSet;
        typename StdSet::iterator m_it;
    };
    iterator begin(void) const { return iterator(StdSet::begin()); }
    iterator end(void) const { return iterator(StdSet::end()); }

    bool contains(T *o) const
    {
        return GCWeakObjectSetImpl::contains(T::GCCast(o));
    }
    void emplace(T *o)
    {
        GCWeakObjectSetImpl::emplace(T::GCCast(o));
    }
    void erase(T *o)
    {
        GCWeakObjectSetImpl::erase(T::GCCast(o));
    }
    iterator erase(iterator it)
    {
        return iterator(GCWeakObjectSetImpl::erase(it.m_it));
    }

    std::vector<T *> GetSnapshot(void) const
    {
        std::vector<T *> ret;
        for (auto it = StdSet::begin(); StdSet::end() != it; ++it)
        {
            GCObject *o = *it;
            ret.emplace_back(static_cast<T *>(o));
        }
        return ret;
    }
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_GC_WEAK_OBJECT_SET_H
