// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: gc_pool.cpp
// Description: GCPool Class
//      Author: Ziming Li
//     Created: 2020-03-15
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "gc_pool.h"

#include "third_party/blink/renderer/core/dom/node.h"

using namespace blink;

namespace BlinKit {

static const char GCPoolKey[] = "GCPool";

GCPool::GCPool(duk_context *ctx) : m_ctx(ctx)
{
    if (nullptr == m_ctx)
        return;

    duk_push_heap_stash(m_ctx);
    if (!duk_get_prop_string(m_ctx, -1, GCPoolKey))
    {
        duk_push_pointer(m_ctx, this);
        duk_put_prop_string(m_ctx, -3, GCPoolKey);
        // Stack: ... stash undefined
    }
    else
    {
        m_active = false; // Another pool is attached before.
        // Stack: ... stash pool
    }
    duk_pop_2(m_ctx);
}

GCPool::~GCPool(void)
{
    if (!m_active)
        return;

    if (nullptr != m_ctx)
    {
        duk_push_heap_stash(m_ctx);
        duk_del_prop_string(m_ctx, -1, GCPoolKey);
        duk_pop(m_ctx);

        duk_gc(m_ctx, 0);
    }

    CollectGarbage();
}

void GCPool::CollectGarbage(void)
{
    if (m_objects.empty())
        return;

    std::unordered_set<ScriptWrappable *> garbages;
    for (ScriptWrappable *object : m_objects)
    {
        if (!object->IsMarkedForGC())
            continue;

        std::vector<ScriptWrappable *> childGarbages;
        object->GetChildrenForGC(childGarbages);
        if (!childGarbages.empty())
            garbages.insert(childGarbages.begin(), childGarbages.end());
    }
    garbages.insert(m_objects.begin(), m_objects.end());
    m_objects.clear();

    for (ScriptWrappable *object : garbages)
    {
        if (!object->IsRetainedByContext())
            delete object;
    }
}

GCPool* GCPool::From(duk_context *ctx)
{
    GCPool *ret = nullptr;
    duk_push_heap_stash(ctx);
    if (duk_get_prop_string(ctx, -1, GCPoolKey))
        ret = reinterpret_cast<GCPool *>(duk_to_pointer(ctx, -1));
    else
        NOTREACHED();
    // Stack: ... stash pool/undefined
    duk_pop_2(ctx);
    return ret;
}

void GCPool::Save(ScriptWrappable &object)
{
    ASSERT(ScriptWrappable::GC_IN_POOL == object.GetGCType());
    ASSERT(!object.IsRetainedByContext());
    ASSERT(object.IsMarkedForGC());
    m_objects.insert(&object);
}

void GCPool::Save(const std::vector<Node *> &detachedNodes)
{
    for (Node *node : detachedNodes)
    {
        if (nullptr != node->ParentOrShadowHostNode())
        {
            ASSERT(!node->IsMarkedForGC());
            continue;
        }

        node->SetGarbageFlag();
        if (node->IsRetainedByContext())
            continue;

        Save(*node);
    }
}

} // namespace BlinKit
