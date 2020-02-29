// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_script_object.cpp
// Description: DukScriptObject Class
//      Author: Ziming Li
//     Created: 2020-01-31
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "duk_script_object.h"

#include "third_party/blink/renderer/platform/bindings/script_wrappable.h"

using namespace blink;

namespace BlinKit {

static const char NativeThis[] = DUK_HIDDEN_SYMBOL("nativeThis");

namespace Impl {

static duk_ret_t ToString(duk_context *ctx)
{
    std::string ret("[object ");

    duk_push_this(ctx);
    duk_get_prototype(ctx, -1);
    if (duk_get_prop_string(ctx, -1, PrototypeEntry::NameKey))
    {
        size_t l = 0;
        const char *s = duk_get_lstring(ctx, -1, &l);
        ret.append(s, l);
    }
    else
    {
        ret.append("Unknown");
    }
    ret.push_back(']');
    duk_push_lstring(ctx, ret.data(), ret.length());
    return 1;
}

} // namespace Impl

void DukScriptObject::BindScriptWrappable(duk_context *ctx, duk_idx_t idx, blink::ScriptWrappable &nativeObject)
{
    idx = duk_normalize_index(ctx, idx);

    ASSERT(nullptr == nativeObject.m_contextObject);
    nativeObject.m_contextObject = duk_get_heapptr(ctx, idx);

    duk_push_pointer(ctx, &nativeObject);
    duk_put_prop_string(ctx, idx, NativeThis);
}

duk_ret_t DukScriptObject::DefaultFinalizer(duk_context *ctx)
{
    ScriptWrappable *nativeThis = DukScriptObject::ToScriptWrappable(ctx, 0);
    if (nullptr != nativeThis)
    {
        if (nativeThis->m_ownedByContext)
            delete nativeThis;
        else
            nativeThis->m_contextObject = nullptr;
    }
    return 0;
}

void DukScriptObject::FillPrototypeEntry(PrototypeEntry &entry)
{
    static const PrototypeEntry::Method Methods[] = {
        { "toString", Impl::ToString, 0 },
    };

    entry.SetFinalizer(DefaultFinalizer);
    entry.Add(Methods, std::size(Methods));
}

duk_idx_t DukScriptObject::PushScriptWrappable(duk_context *ctx, const char *protoName, ScriptWrappable *nativeObject)
{
    if (nullptr == nativeObject)
    {
        duk_push_undefined(ctx);
        return duk_get_top_index(ctx);
    }

    if (nullptr != nativeObject->m_contextObject)
        return duk_push_heapptr(ctx, nativeObject->m_contextObject);
    return PrototypeHelper::CreateScriptObject(ctx, protoName, nativeObject);
}

ScriptWrappable* DukScriptObject::ToScriptWrappable(duk_context *ctx, duk_idx_t idx)
{
    ScriptWrappable *ret = nullptr;
    if (duk_get_prop_string(ctx, idx, NativeThis))
        ret = reinterpret_cast<ScriptWrappable *>(duk_get_pointer(ctx, -1));
    duk_pop(ctx);
    return ret;
}

} // namespace BlinKit
