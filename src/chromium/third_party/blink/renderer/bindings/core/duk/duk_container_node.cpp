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

#include "duk_container_node.h"

#include "third_party/blink/renderer/bindings/core/duk/duk.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_html_collection.h"

using namespace blink;

namespace BlinKit {

namespace Impl {

static duk_ret_t GetElementsByTagName(duk_context *ctx)
{
    const AtomicString name = Duk::To<AtomicString>(ctx, 0);

    duk_push_this(ctx);
    ContainerNode *node = DukScriptObject::To<ContainerNode>(ctx, -1);

    HTMLCollection *ret = node->getElementsByTagName(name);
    DukHTMLCollection::Push(ctx, ret);
    return 1;
}

} // namespace Impl

void DukContainerNode::FillPrototypeEntry(PrototypeEntry &entry)
{
    static const PrototypeEntry::Method Methods[] = {
        { "getElementsByTagName",   Impl::GetElementsByTagName,   1 },
    };

    DukNode::FillPrototypeEntry(entry);
    entry.Add(Methods, std::size(Methods));
}

} // namespace BlinKit
