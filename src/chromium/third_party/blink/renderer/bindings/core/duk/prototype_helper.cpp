// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: prototype_helper.cpp
// Description: PrototypeHelper Class
//      Author: Ziming Li
//     Created: 2020-01-22
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "prototype_helper.h"

#include "third_party/blink/renderer/bindings/core/duk/duk_script_object.h"
#include "third_party/blink/renderer/platform/bindings/script_wrappable.h"

using namespace blink;

namespace BlinKit {

static const char Prototypes[] = "prototypes";

PrototypeHelper::PrototypeHelper(duk_context *ctx)
    : m_ctx(ctx)
#ifndef NDEBUG
    , m_heapPtr(duk_get_heapptr(ctx, -1))
#endif
{
    duk_push_bare_object(m_ctx);
}

PrototypeHelper::~PrototypeHelper(void)
{
#ifndef NDEBUG
    ASSERT(duk_get_heapptr(m_ctx, -2) == m_heapPtr);
#endif
    duk_put_prop_string(m_ctx, -2, Prototypes);
}

bool PrototypeHelper::AttachToScriptObject(duk_context *ctx, duk_idx_t idx, const char *protoName)
{
    bool ret = true;
    idx = duk_normalize_index(ctx, idx);

    const duk_idx_t top = duk_get_top(ctx);
    // ... obj
    duk_push_heap_stash(ctx);
    // ... obj stash
    duk_get_prop_string(ctx, -1, Prototypes);
    // ... obj stash prototypes
    duk_get_prop_string(ctx, -1, protoName);
    // ... obj stash prototypes proto
    if (duk_is_object(ctx, -1))
        duk_set_prototype(ctx, idx);
    else
        ret = false;
    duk_set_top(ctx, top);

    return ret;
}

duk_idx_t PrototypeHelper::CreateScriptObject(duk_context *ctx, const char *protoName, ScriptWrappable *nativeObject)
{
    duk_push_object(ctx);

    if (nullptr != nativeObject)
        DukScriptObject::BindNative(ctx, -1, *nativeObject);

    const duk_idx_t top = duk_get_top(ctx);
    // ... obj
    duk_push_heap_stash(ctx);
    // ... obj stash
    duk_get_prop_string(ctx, -1, Prototypes);
    // ... obj stash prototypes
    duk_get_prop_string(ctx, -1, protoName);
    // ... obj stash prototypes proto
    if (duk_is_object(ctx, -1))
    {
        duk_set_prototype(ctx, -4);
        // ... obj[proto] stash prototypes
        duk_set_top(ctx, top);
        // ... obj[proto]
    }
    else
    {
        ASSERT(duk_is_object(ctx, -1));
        duk_set_top(ctx, top - 1);
        duk_push_undefined(ctx);
    }
    return duk_get_top_index(ctx);
}

void PrototypeHelper::Register(const char *protoName, Worker worker)
{
    PrototypeEntry proto(m_ctx, protoName);
    worker(proto);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static const duk_uint_t CommonFlags =
    DUK_DEFPROP_HAVE_ENUMERABLE
    | DUK_DEFPROP_ENUMERABLE
    | DUK_DEFPROP_HAVE_CONFIGURABLE;

const char PrototypeEntry::NameKey[] = DUK_HIDDEN_SYMBOL("name");

PrototypeEntry::PrototypeEntry(duk_context *ctx, const char *name)
    : m_ctx(ctx), m_name(name)
#ifndef NDEBUG
    , m_top(duk_get_top(ctx))
#endif
{
}

PrototypeEntry::~PrototypeEntry(void)
{
#ifndef NDEBUG
    ASSERT(duk_get_top(m_ctx) == m_top);
#endif

    duk_idx_t idx = duk_push_bare_object(m_ctx);

    duk_push_string(m_ctx, m_name);
    duk_put_prop_string(m_ctx, idx, NameKey);

    if (nullptr != m_finalizer)
    {
        duk_push_c_function(m_ctx, m_finalizer, 1);
        duk_set_finalizer(m_ctx, idx);
    }

    for (const auto &it : m_methods)
    {
        duk_push_lstring(m_ctx, it.first.data(), it.first.length());
        duk_push_c_function(m_ctx, it.second.impl, it.second.argc);
        duk_def_prop(m_ctx, idx, it.second.flags);
    }

    for (const auto &it : m_properties)
    {
        duk_push_lstring(m_ctx, it.first.data(), it.first.length());
        duk_push_c_function(m_ctx, it.second.getter, 0);
        if (nullptr != it.second.setter)
            duk_push_c_function(m_ctx, it.second.setter, 1);
        duk_def_prop(m_ctx, idx, it.second.flags);
    }

    for (const auto &it : m_simpleMembers)
    {
        duk_push_lstring(m_ctx, it.first.data(), it.first.length());

        duk_uint_t extraFlags = DUK_DEFPROP_HAVE_VALUE;
        switch (it.second)
        {
            case DUK_TYPE_OBJECT:
                duk_push_object(m_ctx);
                break;
            default:
                extraFlags = 0;
        }
        duk_def_prop(m_ctx, idx, CommonFlags | extraFlags);
    }

    duk_put_prop_string(m_ctx, -2, m_name);
}

void PrototypeEntry::Add(const Property *properties, size_t count, duk_uint_t extraFlags)
{
    for (size_t i = 0; i < count; ++i)
    {
        const Property &entry = properties[i];

        PropertyData data;
        data.getter = entry.getter;
        data.setter = entry.setter;
        data.flags = DUK_DEFPROP_HAVE_GETTER | CommonFlags | extraFlags;
        if (nullptr != data.setter)
            data.flags |= DUK_DEFPROP_HAVE_SETTER;
        m_properties[entry.name] = data;
    }
}

void PrototypeEntry::Add(const Method *methods, size_t count, duk_uint_t extraFlags)
{
    for (size_t i = 0; i < count; ++i)
    {
        const Method &entry = methods[i];

        MethodData data;
        data.impl = entry.impl;
        data.argc = entry.argc;
        data.flags = DUK_DEFPROP_HAVE_VALUE | DUK_DEFPROP_HAVE_WRITABLE | CommonFlags | extraFlags;
        m_methods[entry.name] = data;
    }
}

} // namespace BlinKit
