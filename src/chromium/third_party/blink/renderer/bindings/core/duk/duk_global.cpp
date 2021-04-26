// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_global.cpp
// Description: DukGlobal Class
//      Author: Ziming Li
//     Created: 2020-07-30
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "duk_global.h"

#include "third_party/blink/renderer/bindings/core/duk/duk_console.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_xhr.h"

namespace BlinKit {

static const duk_uint_t CommonFlags = DUK_DEFPROP_HAVE_ENUMERABLE | DUK_DEFPROP_ENUMERABLE | DUK_DEFPROP_HAVE_CONFIGURABLE;

namespace Impl {

static duk_ret_t AToB(duk_context *ctx)
{
    duk_dup(ctx, 0);
    duk_base64_decode(ctx, -1);
    duk_buffer_to_string(ctx, -1);
    return 1;
}

static duk_ret_t BToA(duk_context *ctx)
{
    duk_dup(ctx, 0);
    duk_base64_encode(ctx, -1);
    duk_buffer_to_string(ctx, -1);
    return 1;
}

static duk_ret_t ConsoleGetter(duk_context *ctx)
{
    DukScriptObject::Create<DukConsole>(ctx);
    return 1;
}

} // namespace Impl

void DukGlobal::AddMethod(duk_context *ctx, duk_idx_t dst, const char *name, duk_c_function impl, duk_idx_t nargs)
{
    ASSERT(duk_normalize_index(ctx, dst) == dst);
    duk_push_string(ctx, name);
    duk_push_c_function(ctx, impl, nargs);
    duk_def_prop(ctx, dst, DUK_DEFPROP_HAVE_VALUE | DUK_DEFPROP_HAVE_WRITABLE | CommonFlags);
}

void DukGlobal::AddProperty(duk_context *ctx, duk_idx_t dst, const char *name, duk_c_function getter, duk_c_function setter)
{
    duk_uint_t flags = DUK_DEFPROP_HAVE_GETTER | CommonFlags;

    ASSERT(duk_normalize_index(ctx, dst) == dst);
    duk_push_string(ctx, name);
    duk_push_c_function(ctx, getter, 0);
    if (nullptr != setter)
    {
        flags |= DUK_DEFPROP_HAVE_SETTER;
        duk_push_c_function(ctx, setter, 1);
    }
    duk_def_prop(ctx, dst, flags);
}

void DukGlobal::Attach(duk_context *ctx, duk_idx_t dst)
{
    ASSERT(dst >= 0);

    AddMethod(ctx, dst, "atob", Impl::AToB, 1);
    AddMethod(ctx, dst, "btoa", Impl::BToA, 1);
    AddMethod(ctx, dst, DukXHR::ProtoName, DukXHR::Construct, 0);

    AddProperty(ctx, dst, "console", Impl::ConsoleGetter);

    duk_push_object(ctx);
    duk_put_prop_string(ctx, dst, "external");
}

} // namespace BlinKit
