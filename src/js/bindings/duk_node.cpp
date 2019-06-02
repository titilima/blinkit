// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_node.cpp
// Description: Bindings for Node
//      Author: Ziming Li
//     Created: 2019-06-01
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "duk_node.h"

#include "context/prototype_manager.h"
#include "wrappers/duk.h"

using namespace blink;

namespace BlinKit {

void DukNode::RegisterToPrototypeEntry(PrototypeEntry &entry)
{
    static const PrototypeEntry::Property Properties[] = {
        { "textContent", TextContentGetter, TextContentSetter },
    };

    DukEventTarget::RegisterToPrototypeEntry(entry);
    entry.Add(Properties, WTF_ARRAY_LENGTH(Properties));
}

duk_ret_t DukNode::TextContentGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Node *node = DukEventTarget::GetNativeThis<Node>(ctx);
    return Duk::PushString(ctx, node->textContent());
}

duk_ret_t DukNode::TextContentSetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Node *node = DukEventTarget::GetNativeThis<Node>(ctx);
    node->setTextContent(Duk::ToWTFString(ctx, 0));
    return 0;
}

} // namespace BlinKit
