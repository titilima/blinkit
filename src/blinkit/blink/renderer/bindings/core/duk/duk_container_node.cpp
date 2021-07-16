// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_container_node.cpp
// Description: DukContainerNode Class
//      Author: Ziming Li
//     Created: 2020-02-07
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "./duk_container_node.h"

#include "blinkit/blink/renderer/bindings/core/duk/duk.h"
#include "blinkit/blink/renderer/bindings/core/duk/duk_element.h"
#include "blinkit/blink/renderer/bindings/core/duk/duk_html_collection.h"
#include "blinkit/blink/renderer/bindings/core/duk/duk_node_list.h"
#include "blinkit/blink/renderer/core/dom/StaticNodeList.h"
#include "blinkit/blink/renderer/core/dom/TagCollection.h"

using namespace blink;

namespace BlinKit {

namespace Impl {

static duk_ret_t GetElementsByTagName(duk_context *ctx)
{
    const AtomicString name = Duk::To<AtomicString>(ctx, 0);

    duk_push_this(ctx);
    ContainerNode *node = DukScriptObject::To<ContainerNode>(ctx, -1);

    TagCollection *ret = node->getElementsByTagName(name);
    DukHTMLCollection::Push(ctx, ret);
    return 1;
}

static duk_ret_t QuerySelector(duk_context *ctx)
{
    const AtomicString selectors = Duk::To<AtomicString>(ctx, 0);

    duk_push_this(ctx);
    ContainerNode *node = DukScriptObject::To<ContainerNode>(ctx, -1);

    DukExceptionState exceptionState(ctx);
    Element *ret = node->querySelector(selectors, exceptionState);
    if (exceptionState.hadException())
    {
        exceptionState.ThrowIfNeeded();
        return 0;
    }

    DukElement::Push(ctx, ret);
    return 1;
}

static duk_ret_t QuerySelectorAll(duk_context *ctx)
{
    const AtomicString selectors = Duk::To<AtomicString>(ctx, 0);

    duk_push_this(ctx);
    ContainerNode *node = DukScriptObject::To<ContainerNode>(ctx, -1);

    DukExceptionState exceptionState(ctx);
    StaticElementList *ret = node->querySelectorAll(selectors, exceptionState);
    if (exceptionState.hadException())
    {
        exceptionState.ThrowIfNeeded();
        return 0;
    }

    DukNodeList::Push(ctx, ret);
    return 1;
}

} // namespace Impl

void DukContainerNode::FillPrototypeEntry(PrototypeEntry &entry)
{
    static const PrototypeEntry::Method Methods[] = {
        { "getElementsByTagName",   Impl::GetElementsByTagName,   1 },
        { "querySelector",          Impl::QuerySelector,          1 },
        { "querySelectorAll",       Impl::QuerySelectorAll,       1 },
    };

    DukNode::FillPrototypeEntry(entry);
    entry.Add(Methods, std::size(Methods));
}

} // namespace BlinKit
