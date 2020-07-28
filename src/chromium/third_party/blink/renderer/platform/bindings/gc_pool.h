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
#include <vector>
#include "duktape/duktape.h"

namespace blink {
class Node;
class ScriptWrappable;
}

namespace BlinKit {

class GCPool final
{
public:
    GCPool(duk_context *ctx = nullptr);
    ~GCPool(void);
    static GCPool* From(duk_context *ctx);

    void CollectGarbage(void);
    void Save(blink::ScriptWrappable &object);
    void Save(const std::vector<blink::Node *> &detachedNodes);

    void DetachContext(void) { m_ctx = nullptr; }
private:
    duk_context *m_ctx;
    bool m_active = true;
    std::unordered_set<blink::ScriptWrappable *> m_objects;
};

} // namespace BlinKit

#endif // BLINKIT_BLINK_GC_POOL_H
