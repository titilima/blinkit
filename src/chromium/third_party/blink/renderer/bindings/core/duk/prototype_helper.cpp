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

#include "third_party/blink/renderer/platform/bindings/script_wrappable.h"

using namespace blink;

namespace BlinKit {

static const char Prototypes[] = "prototypes";

PrototypeHelper::PrototypeHelper(duk_context *ctx)
    : m_ctx(ctx)
#ifdef _DEBUG
    , m_heapPtr(duk_get_heapptr(ctx, -1))
#endif
{
    duk_push_bare_object(m_ctx);
}

PrototypeHelper::~PrototypeHelper(void)
{
    ASSERT(duk_get_heapptr(m_ctx, -2) == m_heapPtr);
    duk_put_prop_string(m_ctx, -2, Prototypes);
}

duk_idx_t PrototypeHelper::CreateScriptObject(duk_context *ctx, const char *protoName, ScriptWrappable &nativeObject)
{
    ASSERT(nullptr == nativeObject.m_contextObject);

    duk_push_object(ctx);

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

PrototypeEntry::PrototypeEntry(duk_context *ctx, const char *name)
    : m_ctx(ctx), m_name(name)
#ifdef _DEBUG
    , m_heapPtr(duk_get_heapptr(ctx, -1))
#endif
{
    duk_push_bare_object(m_ctx);
}

PrototypeEntry::~PrototypeEntry(void)
{
    ASSERT(duk_get_heapptr(m_ctx, -2) == m_heapPtr);
    duk_put_prop_string(m_ctx, -2, m_name);
}

void PrototypeEntry::Add(const Property *properties, size_t count, duk_uint_t extraFlags)
{
    ASSERT(duk_get_heapptr(m_ctx, -2) == m_heapPtr);

    const duk_idx_t idx = duk_normalize_index(m_ctx, -1);
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
        duk_def_prop(m_ctx, idx, flags);
    }
}

void PrototypeEntry::Add(const Method *methods, size_t count, duk_uint_t extraFlags)
{
    ASSERT(duk_get_heapptr(m_ctx, -2) == m_heapPtr);

    const duk_idx_t idx = duk_normalize_index(m_ctx, -1);
    const duk_uint_t flags = DUK_DEFPROP_HAVE_VALUE | DUK_DEFPROP_HAVE_WRITABLE | CommonFlags | extraFlags;
    for (size_t i = 0; i < count; ++i)
    {
        const Method &entry = methods[i];

        duk_push_string(m_ctx, entry.name);
        duk_push_c_function(m_ctx, entry.impl, entry.argc);
        duk_def_prop(m_ctx, idx, flags);
    }
}

void PrototypeEntry::SetFinalizer(duk_c_function finalizer)
{
    ASSERT(duk_get_heapptr(m_ctx, -2) == m_heapPtr);

    duk_push_c_function(m_ctx, finalizer, 1);
    duk_set_finalizer(m_ctx, -2);
}

} // namespace BlinKit
