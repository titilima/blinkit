// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_container_node.cpp
// Description: Bindings for ContainerNode
//      Author: Ziming Li
//     Created: 2019-06-17
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "duk_container_node.h"

#include "core/dom/ClassCollection.h"
#include "core/dom/Element.h"
#include "core/dom/TagCollection.h"

#include "bindings/duk_exception_state.h"
#include "bindings/duk_html_collection.h"
#include "bindings/duk_node_list.h"
#include "context/duk_context.h"
#include "context/prototype_manager.h"
#include "wrappers/duk.h"

using namespace blink;

namespace BlinKit {

namespace Impl {

static duk_ret_t GetElementsByClassName(duk_context *ctx)
{
    duk_push_this(ctx);
    ContainerNode *node = DukEventTarget::GetNativeThis<ContainerNode>(ctx);

    PassRefPtr<ClassCollection> ret = node->getElementsByClassName(Duk::ToAtomicString(ctx, 0));
    DukContext::From(ctx)->PushObject<DukHTMLCollection>(ctx, ret.get());
    return 1;
}

static duk_ret_t GetElementsByTagName(duk_context *ctx)
{
    duk_push_this(ctx);
    ContainerNode *node = DukEventTarget::GetNativeThis<ContainerNode>(ctx);

    PassRefPtr<TagCollection> ret = node->getElementsByTagName(Duk::ToAtomicString(ctx, 0));
    DukContext::From(ctx)->PushObject<DukHTMLCollection>(ctx, ret.get());
    return 1;
}

static duk_ret_t QuerySelector(duk_context *ctx)
{
    duk_push_this(ctx);
    ContainerNode *node = DukEventTarget::GetNativeThis<ContainerNode>(ctx);

    DukExceptionState es(ctx, "querySelector", "ContainerNode");
    PassRefPtr<Element> ret = node->querySelector(Duk::ToAtomicString(ctx, 0), es);
    if (es.hadException())
    {
        es.throwIfNeeded();
        return 0;
    }

    DukContext::From(ctx)->PushNode(ctx, ret.get());
    return 1;
}

static duk_ret_t QuerySelectorAll(duk_context *ctx)
{
    duk_push_this(ctx);
    ContainerNode *node = DukEventTarget::GetNativeThis<ContainerNode>(ctx);

    DukExceptionState es(ctx, "querySelectorAll", "ContainerNode");
    PassRefPtr<StaticElementList> ret = node->querySelectorAll(Duk::ToAtomicString(ctx, 0), es);
    if (es.hadException())
    {
        es.throwIfNeeded();
        return 0;
    }

    DukContext::From(ctx)->PushObject<DukNodeList>(ctx, ret.get());
    return 1;
}

} // namespace Impl

void DukContainerNode::RegisterToPrototypeEntry(PrototypeEntry &entry)
{
    static const PrototypeEntry::Method Methods[] = {
        { "getElementsByClassName", Impl::GetElementsByClassName, 1 },
        { "getElementsByTagName",   Impl::GetElementsByTagName,   1 },
        { "querySelector",          Impl::QuerySelector,          1 },
        { "querySelectorAll",       Impl::QuerySelectorAll,       1 },
    };

    DukNode::RegisterToPrototypeEntry(entry);
    entry.Add(Methods, WTF_ARRAY_LENGTH(Methods));
}

} // namespace BlinKit
