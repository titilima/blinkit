// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_named_node_map.cpp
// Description: DukNamedNodeMap Class
//      Author: Ziming Li
//     Created: 2020-03-18
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "./duk_named_node_map.h"

#include "blinkit/blink/renderer/bindings/core/duk/duk.h"
#include "blinkit/blink/renderer/bindings/core/duk/duk_element.h"
#include "blinkit/blink/renderer/bindings/core/duk/script_wrappers.h"
#include "blinkit/blink/renderer/core/dom/Attr.h"

using namespace blink;

namespace BlinKit {

static duk_ret_t ProxyGet(duk_context *ctx)
{
    NamedNodeMap *namedNodeMap = DukScriptObject::To<NamedNodeMap>(ctx, 0);

    if (duk_is_number(ctx, 1))
    {
        Attr *ret = namedNodeMap->item(duk_to_uint(ctx, 1));
        DukNode::Push(ctx, ret);
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
        duk_push_uint(ctx, namedNodeMap->length());
        return 1;
    }

    const AtomicString name = AtomicString::fromStdUTF8(k);
    Attr *ret = namedNodeMap->getNamedItem(name);
    if (nullptr != ret)
    {
        DukNode::Push(ctx, ret);
        return 1;
    }

    ASSERT(false); // BKTODO:
    return 0;
}

void DukNamedNodeMap::InitializeProxy(duk_context *ctx)
{
    duk_push_bare_object(ctx);
    duk_push_c_function(ctx, ProxyGet, 2);
    duk_put_prop_string(ctx, -2, "get");
    duk_push_proxy(ctx, 0);
}

duk_idx_t DukNamedNodeMap::Push(duk_context *ctx, NamedNodeMap *namedNodeMap)
{
    PushWrapper w(ctx, namedNodeMap);
    if (!DukScriptObject::Push(ctx, namedNodeMap))
    {
        duk_push_bare_object(ctx);

        duk_push_c_function(ctx, DukScriptObject::DefaultFinalizer, 1);
        duk_set_finalizer(ctx, -2);

        DukScriptObject::BindNative(ctx, -1, *namedNodeMap);

        InitializeProxy(ctx);
    }
    return duk_get_top_index(ctx);
}

} // namespace BlinKit
