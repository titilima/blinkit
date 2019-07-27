// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_html_collection.cpp
// Description: Bindings for HTMLCollection
//      Author: Ziming Li
//     Created: 2019-07-20
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "duk_html_collection.h"

#include "context/duk_context.h"
#include "context/prototype_manager.h"
#include "wrappers/duk.h"

using namespace blink;

namespace BlinKit {

const char DukHTMLCollection::ProtoName[] = "HTMLCollection";

static HTMLCollection* Get(duk_context *ctx, duk_idx_t idx = -1)
{
    return static_cast<HTMLCollection *>(Duk::GetNativeThis(ctx, idx));
}

duk_ret_t DukHTMLCollection::Finalizer(duk_context *ctx)
{
    HTMLCollection *nativeThis = Get(ctx, 0);
    DukContext::From(ctx)->RemoveObjectFromPool(nativeThis);
    return 0;
}

void DukHTMLCollection::OnPush(duk_context *ctx, ScriptWrappable *)
{
    duk_push_object(ctx);
    duk_push_c_function(ctx, ProxyGet, 3);
    duk_put_prop_string(ctx, -2, "get");
    duk_push_proxy(ctx, 0);
}

duk_ret_t DukHTMLCollection::ProxyGet(duk_context *ctx)
{
    HTMLCollection *nativeThis = Get(ctx, 0);

    if (duk_is_number(ctx, 1))
    {
        duk_uint_t index = duk_to_uint(ctx, 1);
        DukContext::From(ctx)->PushNode(ctx, nativeThis->item(index));
        return 1;
    }

    assert(duk_is_string(ctx, 1));
    const std::string key = Duk::ToString(ctx, 1);
    if (key == "length")
    {
        duk_push_uint(ctx, nativeThis->length());
    }
    else
    {
        assert(false); // BKTODO:
    }

    return 1;
}

void DukHTMLCollection::RegisterPrototype(duk_context *ctx, PrototypeManager &protos)
{
    const auto worker = [](PrototypeEntry &entry)
    {
        entry.SetFinalizer(Finalizer);
    };
    protos.Register(ctx, ProtoName, worker);
}

} // namespace BlinKit
