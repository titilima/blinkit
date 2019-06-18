// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: prototype_manager.cpp
// Description: PrototypeManager Class
//      Author: Ziming Li
//     Created: 2019-05-19
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "prototype_manager.h"

namespace BlinKit {

static const duk_uint_t CommonFlags = DUK_DEFPROP_HAVE_ENUMERABLE | DUK_DEFPROP_ENUMERABLE | DUK_DEFPROP_HAVE_CONFIGURABLE;

PrototypeEntry::PrototypeEntry(duk_context *ctx, duk_idx_t idx) : m_ctx(ctx), m_idx(idx)
{
    // Nothing
}

void PrototypeEntry::Add(const char *name, int value)
{
    duk_push_string(m_ctx, name);
    duk_push_int(m_ctx, value);
    duk_def_prop(m_ctx, m_idx, CommonFlags | DUK_DEFPROP_HAVE_VALUE);
}

void PrototypeEntry::Add(const Property *properties, size_t count, duk_uint_t extraFlags)
{
    const duk_uint_t baseFlags = DUK_DEFPROP_HAVE_GETTER | CommonFlags | extraFlags;
    for (size_t i = 0; i < count; ++i)
    {
        const Property &entry = properties[i];

        duk_uint_t flags = baseFlags;

        duk_push_string(m_ctx, entry.name);
        duk_push_c_function(m_ctx, entry.getter, 0);
        if (nullptr != entry.setter)
        {
            flags |= DUK_DEFPROP_HAVE_SETTER;
            duk_push_c_function(m_ctx, entry.setter, 1);
        }
        duk_def_prop(m_ctx, m_idx, flags);
    }
}

void PrototypeEntry::Add(const Method *methods, size_t count, duk_uint_t extraFlags)
{
    const duk_uint_t flags = DUK_DEFPROP_HAVE_VALUE | DUK_DEFPROP_HAVE_WRITABLE | CommonFlags | extraFlags;
    for (size_t i = 0; i < count; ++i)
    {
        const Method &entry = methods[i];

        duk_push_string(m_ctx, entry.name);
        duk_push_c_function(m_ctx, entry.impl, entry.argc);
        duk_def_prop(m_ctx, m_idx, flags);
    }
}

void PrototypeEntry::AddObject(const char *name)
{
    duk_push_string(m_ctx, name);
    duk_push_object(m_ctx);
    duk_def_prop(m_ctx, m_idx, CommonFlags | DUK_DEFPROP_HAVE_VALUE);
}

void PrototypeEntry::SetFinalizer(duk_c_function finalizer)
{
    duk_push_c_function(m_ctx, finalizer, 1);
    duk_set_finalizer(m_ctx, m_idx);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

PrototypeManager::PrototypeManager(duk_context *ctx)
{
    duk_push_array(ctx);
    m_protosPtr = duk_get_heapptr(ctx, -1);
    duk_put_prop_string(ctx, -2, "prototypes");
}

void PrototypeManager::BeginRegisterTransaction(duk_context *ctx)
{
    duk_push_heapptr(ctx, m_protosPtr);
}

bool PrototypeManager::Bind(duk_context *ctx, const char *protoName, duk_idx_t idx)
{
    assert(duk_is_object(ctx, idx));
    idx = duk_normalize_index(ctx, idx);

    auto it = m_protos.find(protoName);
    if (std::end(m_protos) == it)
    {
        assert(std::end(m_protos) != it);
        return false;
    }

    duk_push_heapptr(ctx, it->second);
    duk_set_prototype(ctx, idx);
    return true;
}

bool PrototypeManager::CreateObject(duk_context *ctx, const char *protoName)
{
    auto it = m_protos.find(protoName);
    if (std::end(m_protos) == it)
    {
        assert(std::end(m_protos) != it);
        return false;
    }

    duk_push_object(ctx);
    duk_push_heapptr(ctx, it->second);
    duk_set_prototype(ctx, -2);
#if _BK_VERBOSE_LOG
    BKLOG("Object `%s` created: %x", protoName, duk_get_heapptr(ctx, -1));
#endif
    return true;
}

void PrototypeManager::EndRegisterTransaction(duk_context *ctx)
{
    assert(duk_get_heapptr(ctx, -1) == m_protosPtr);
    duk_pop(ctx);
}

bool PrototypeManager::Register(duk_context *ctx, const char *protoName, const std::function<void(PrototypeEntry &)> &worker)
{
    assert(duk_get_heapptr(ctx, -1) == m_protosPtr);

    auto it = m_protos.find(protoName);
    if (std::end(m_protos) != it)
    {
        assert(std::end(m_protos) == it);
        return false;
    }

    const duk_size_t size = duk_get_length(ctx, -1);
    assert(m_protos.size() == size);

    duk_push_object(ctx);
    m_protos[protoName] = duk_get_heapptr(ctx, -1);

    const duk_idx_t topIdx = duk_get_top_index(ctx);
    PrototypeEntry entry(ctx, topIdx);
    worker(entry);

    assert(duk_get_top_index(ctx) == topIdx);
    duk_put_prop_index(ctx, -2, size);
    return true;
}

} // namespace BlinKit
