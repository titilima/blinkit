// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_named_node_map.cpp
// Description: Bindings for NamedNodeMap
//      Author: Ziming Li
//     Created: 2019-07-22
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "duk_named_node_map.h"

#include "core/dom/Attr.h"

#include "context/duk_context.h"
#include "context/prototype_manager.h"
#include "wrappers/duk.h"

using namespace blink;

namespace BlinKit {

const char DukNamedNodeMap::ProtoName[] = "NamedNodeMap";

static NamedNodeMap* Get(duk_context *ctx, duk_idx_t idx = -1)
{
    return static_cast<NamedNodeMap *>(Duk::GetNativeThis(ctx, idx));
}

duk_ret_t DukNamedNodeMap::Finalizer(duk_context *ctx)
{
    NamedNodeMap *nativeThis = Get(ctx, 0);
    DukContext::From(ctx)->RemoveObjectFromPool(nativeThis);
    return 0;
}

void DukNamedNodeMap::OnPush(duk_context *ctx, ScriptWrappable *)
{
    duk_push_object(ctx);
    duk_push_c_function(ctx, ProxyGet, 3);
    duk_put_prop_string(ctx, -2, "get");
    duk_push_proxy(ctx, 0);
}

namespace Impl {

} // namespace Impl

duk_ret_t DukNamedNodeMap::ProxyGet(duk_context *ctx)
{
    NamedNodeMap *nativeThis = Get(ctx, 0);

    if (duk_is_number(ctx, 1))
    {
        assert(false); // BKTODO:
    }

    assert(duk_is_string(ctx, 1));
    const std::string key = Duk::ToString(ctx, 1);
    if (key == "getNamedItem")
        assert(false); // BKTODO:
    else if (key == "setNamedItem")
        assert(false); // BKTODO:
    else if (key == "removeNamedItem")
        assert(false); // BKTODO:
    else if (key == "item")
        assert(false); // BKTODO:

    PassRefPtr<Attr> ret = nativeThis->getNamedItem(AtomicString::fromUTF8(key.data(), key.length()));
    if (!ret)
        return 0;

    DukContext::From(ctx)->PushNode(ctx, ret.get());
    return 1;
}

void DukNamedNodeMap::RegisterPrototype(duk_context *ctx, PrototypeManager &protos)
{
    const auto worker = [](PrototypeEntry &entry)
    {
        entry.SetFinalizer(Finalizer);
    };
    protos.Register(ctx, ProtoName, worker);
}

} // namespace BlinKit
