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

#include "./duk_script_object.h"

#include "blinkit/blink/renderer/bindings/core/duk/script_wrappable.h"
#include "blinkit/blink/renderer/bindings/core/duk/script_wrappers.h"
#include "blinkit/js/context_impl.h"

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

void DukScriptObject::BindNative(duk_context *ctx, duk_idx_t idx, ScriptWrappable &nativeObject)
{
    idx = duk_normalize_index(ctx, idx);

    duk_push_pointer(ctx, &nativeObject);
    duk_put_prop_string(ctx, idx, NativeThis);
}

duk_ret_t DukScriptObject::DefaultFinalizer(duk_context *ctx)
{
    ScriptWrappable *nativeThis = DukScriptObject::ToScriptWrappable(ctx, 0);
    if (nullptr != nativeThis)
    {
        nativeThis->m_contextObject = nullptr;
        if (GCObject *o = nativeThis->ObjectForGC())
            o->Release();
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

bool DukScriptObject::Push(duk_context *ctx, ScriptWrappable *nativeObject)
{
    if (nullptr == nativeObject)
    {
        duk_push_undefined(ctx);
        return true;
    }

    if (nullptr != nativeObject->m_contextObject)
    {
        duk_push_heapptr(ctx, nativeObject->m_contextObject);
        return true;
    }

    return false;
}

duk_idx_t DukScriptObject::PushScriptWrappable(duk_context *ctx, const char *protoName, ScriptWrappable *nativeObject)
{
    PushWrapper _(ctx, nativeObject);
    if (Push(ctx, nativeObject))
        return duk_get_top_index(ctx);
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
