// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_node_list.cpp
// Description: DukNodeList Class
//      Author: Ziming Li
//     Created: 2020-03-31
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "duk_node_list.h"

#include "third_party/blink/renderer/bindings/core/duk/duk_node.h"

using namespace blink;

namespace BlinKit {

void DukNodeList::PushItem(duk_context *ctx, NodeList *nodeList, duk_uarridx_t index)
{
    DukNode::Push(ctx, nodeList->item(index));
}

} // namespace BlinKit
