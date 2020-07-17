// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: heap_retained.cpp
// Description: HeapRetained Class
//      Author: Ziming Li
//     Created: 2020-07-14
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "heap_retained.h"


namespace BlinKit {

HeapRetained::HeapRetained(const std::string &keyPrefix) : m_heapKey(keyPrefix)
{
}

HeapRetained::~HeapRetained(void)
{
    ASSERT(nullptr == m_heapPtr);
}

duk_idx_t HeapRetained::PushTo(duk_context *ctx) const
{
    ASSERT(nullptr != m_heapPtr);
    return duk_push_heapptr(ctx, m_heapPtr);
}

void HeapRetained::Release(duk_context *ctx)
{
    duk_push_global_object(ctx);
    duk_del_prop_lstring(ctx, -1, m_heapKey.data(), m_heapKey.length());
    duk_pop(ctx);

    m_heapPtr = nullptr;
}

void HeapRetained::Retain(duk_context *ctx, duk_idx_t idx)
{
    ASSERT(nullptr == m_heapPtr);
    m_heapPtr = duk_get_heapptr(ctx, idx);

    m_heapKey.push_back('_');

    char buf[128];
    sprintf(buf, "%p", m_heapPtr);
    m_heapKey.append(buf);

    duk_push_global_object(ctx);
    duk_push_heapptr(ctx, m_heapPtr);
    duk_put_prop_lstring(ctx, -2, m_heapKey.data(), m_heapKey.length());
    duk_pop(ctx);
}

bool HeapRetained::SafeAccess(duk_context *ctx, const Worker &worker)
{
    bool ret = true;
    const int top = duk_get_top(ctx);
    duk_push_global_object(ctx);
    if (duk_get_prop_lstring(ctx, -1, m_heapKey.data(), m_heapKey.length()))
        worker(ctx, duk_normalize_index(ctx, -1));
    else
        ret = false;
    duk_set_top(ctx, top);
    return ret;
}

HeapRetainedValue::HeapRetainedValue(duk_context *ctx, Type type, const char *keyPrefix) : HeapRetained(keyPrefix), m_ctx(ctx)
{
    switch (type)
    {
        case Type::OBJECT:
            duk_push_bare_object(ctx);
            break;
        case Type::ARRAY:
            duk_push_bare_array(ctx);
            break;
        default:
            NOTREACHED();
            return;
    }
    HeapRetained::Retain(m_ctx, -1);
    duk_pop(m_ctx);
}

HeapRetainedValue::~HeapRetainedValue(void)
{
    HeapRetained::Release(m_ctx);
}

} // namespace BlinKit
