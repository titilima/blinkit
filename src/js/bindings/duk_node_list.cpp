// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_node_list.cpp
// Description: Bindings for NodeList
//      Author: Ziming Li
//     Created: 2019-06-04
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "duk_node_list.h"

#include "bindings/duk_element.h"
#include "context/duk_context.h"
#include "context/prototype_manager.h"
#include "wrappers/duk.h"

using namespace blink;

namespace BlinKit {

const char DukNodeList::ProtoName[] = "NodeList";

static NodeList* Get(duk_context *ctx, duk_idx_t idx = -1)
{
    return reinterpret_cast<NodeList *>(Duk::GetNativeThis(ctx, idx));
}

duk_ret_t DukNodeList::Finalizer(duk_context *ctx)
{
    NodeList *nativeThis = Get(ctx, 0);
    DukContext::From(ctx)->RemoveObjectFromPool(nativeThis);
    return 0;
}

void DukNodeList::OnCreate(duk_context *ctx, ScriptWrappable *nativeThis)
{
    DukObjectImpl::OnCreate(ctx, nativeThis);

    NodeList *nodeList = reinterpret_cast<NodeList *>(nativeThis);

    DukContext *context = DukContext::From(ctx);

    unsigned length = nodeList->length();
    for (unsigned i = 0; i < length; ++i)
    {
        context->PushNode(nodeList->item(i));
        duk_put_prop_index(ctx, -2, i);
    }
}

namespace Impl {

duk_ret_t Item(duk_context *ctx)
{
    duk_push_this(ctx);
    NodeList *nodeList = Get(ctx);

    Node *node = nodeList->item(duk_to_uint(ctx, 0));
    DukContext::From(ctx)->PushNode(node);
    return 1;
}

duk_ret_t LengthGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    NodeList *nodeList = Get(ctx);
    duk_push_uint(ctx, nodeList->length());
    return 1;
}

} // namespace Impl

void DukNodeList::RegisterPrototype(duk_context *ctx, PrototypeManager &protos)
{
    static const PrototypeEntry::Property Properties[] = {
        { "length", Impl::LengthGetter, nullptr }
    };
    static const PrototypeEntry::Method Methods[] = {
        { "item", Impl::Item, 1 }
    };

    const auto worker = [](PrototypeEntry &entry)
    {
        entry.Add(Properties, WTF_ARRAY_LENGTH(Properties));
        entry.Add(Methods, WTF_ARRAY_LENGTH(Methods));
    };
    protos.Register(ctx, ProtoName, worker);
}

} // namespace BlinKit
