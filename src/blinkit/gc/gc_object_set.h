#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: gc_object_set.h
// Description: GCObjectSet Class
//      Author: Ziming Li
//     Created: 2021-08-29
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_GC_OBJECT_SET_H
#define BLINKIT_BLINKIT_GC_OBJECT_SET_H

#include <functional>
#include <unordered_set>
#include "blinkit/gc/gc_def.h"
#include "third_party/zed/include/zed/container_utilites.hpp"

namespace BlinKit {

class GCObjectSetCallback
{
public:
    virtual void EnumObjects(const std::function<void(GCObject &)> &callback) const = 0;
    virtual void Detach(GCObject &o) = 0;
protected:
    GCObjectSetCallback(void) = default;
};

class GCObjectSetImpl : public GCObjectSetCallback, protected std::unordered_set<GCObject *>
{
    using StdSet = std::unordered_set<GCObject *>;
public:
    ~GCObjectSetImpl(void)
    {
        clear();
    }

    using StdSet::empty;

    void clear(void)
    {
        for (auto it = StdSet::begin(); StdSet::end() != it; ++it)
            (*it)->Release();
        StdSet::clear();
    }
protected:
    bool contains(GCObject *o) const
    {
        return zed::key_exists(static_cast<const StdSet &>(*this), o);
    }
    void emplace(GCObject *o)
    {
        o->IncRef();
        StdSet::emplace(o);
    }
    void erase(GCObject *o)
    {
        auto it = StdSet::find(o);
        if (StdSet::end() != it)
        {
            StdSet::erase(it);
            o->Release();
        }
    }
    void EnumObjects(const std::function<void(GCObject &)> &callback) const final
    {
        for (auto it = StdSet::begin(); StdSet::end() != it; ++it)
        {
            GCObject *o = *it;
            callback(*o);
        }
    }
private:
    void Detach(GCObject &o) final
    {
        StdSet::erase(&o);
    }
};

template <class T>
class GCObjectSet final : public GCObjectSetImpl
{
public:
    bool contains(T *o) const
    {
        return GCObjectSetImpl::contains(T::GCCast(o));
    }
    void emplace(T *o)
    {
        GCObjectSetImpl::emplace(T::GCCast(o));
    }
    void erase(T *o)
    {
        GCObjectSetImpl::erase(T::GCCast(o));
    }
    void for_each(const std::function<void(T &)> &callback) const
    {
        GCObjectSetImpl::EnumObjects([&callback](GCObject &o) {
            callback(static_cast<T &>(o));
        });
    }

    std::vector<T *> GetSnapshot(void) const
    {
        std::vector<T *> ret;
        GCObjectSetImpl::EnumObjects([&ret](GCObject &o) {
            ret.emplace_back(static_cast<T *>(&o));
        });
        return ret;
    }
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_GC_OBJECT_SET_H
