// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: gc_pool.h
// Description: GCPool Class
//      Author: Ziming Li
//     Created: 2020-03-15
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_GC_POOL_H
#define BLINKIT_BLINK_GC_POOL_H

#pragma once

#include <unordered_set>

namespace blink {
class Document;
class ScriptWrappable;
}

namespace BlinKit {

class GCPool final
{
public:
    GCPool(void) = default;
    ~GCPool(void) { CollectGarbage(); }

    static GCPool& From(const blink::Document &document);

    void CollectGarbage(void);

    void Save(blink::ScriptWrappable &object);
    void Restore(blink::ScriptWrappable &object);

    template <class T>
    T* Save(T *object)
    {
        if (nullptr != object)
            Save(*object);
        return object;
    }
private:
    std::unordered_set<blink::ScriptWrappable *> m_objects;
};

} // namespace BlinKit

#endif // BLINKIT_BLINK_GC_POOL_H
