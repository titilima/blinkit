// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_html_collection.cpp
// Description: DukHTMLCollection Class
//      Author: Ziming Li
//     Created: 2020-03-07
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "duk_html_collection.h"

#include "third_party/blink/renderer/bindings/core/duk/duk.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_element.h"
#include "third_party/blink/renderer/platform/bindings/script_wrappers.h"

using namespace blink;

namespace BlinKit {

static duk_ret_t ProxyGet(duk_context *ctx)
{
    HTMLCollection *collection = DukScriptObject::To<HTMLCollection>(ctx, 0);

    if (duk_is_number(ctx, 1))
    {
        Element *ret = collection->item(duk_to_uint(ctx, 1));
        DukElement::Push(ctx, ret);
        return 1;
    }

    if (!duk_is_string(ctx, 1))
    {
        NOTREACHED();
        return 0;
    }

    const std::string k = Duk::To<std::string>(ctx, 1);
    if (k == "length")
    {
        duk_push_uint(ctx, collection->length());
    }
    else
    {
        ASSERT(false); // BKTODO:
        return 0;
    }
    return 1;
}

void DukHTMLCollection::InitializeProxy(duk_context *ctx)
{
    duk_push_bare_object(ctx);
    duk_push_c_function(ctx, ProxyGet, 2);
    duk_put_prop_string(ctx, -2, "get");
    duk_push_proxy(ctx, 0);
}

duk_idx_t DukHTMLCollection::Push(duk_context *ctx, HTMLCollection *collection)
{
    PushWrapper w(ctx, collection);
    if (!DukScriptObject::Push(ctx, collection))
    {
        duk_push_bare_object(ctx);

        duk_push_c_function(ctx, DukScriptObject::DefaultFinalizer, 1);
        duk_set_finalizer(ctx, -2);

        DukScriptObject::BindNative(ctx, -1, *collection);

        InitializeProxy(ctx);
    }
    return duk_get_top_index(ctx);
}

} // namespace BlinKit
