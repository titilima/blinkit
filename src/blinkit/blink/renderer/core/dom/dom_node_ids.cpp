// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: dom_node_ids.cpp
// Description: DOMNodeIds Class
//      Author: Ziming Li
//     Created: 2021-08-04
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./dom_node_ids.h"

namespace blink {

static_assert(sizeof(Node *) == sizeof(int));

int DOMNodeIds::idForNode(Node *node)
{
    return reinterpret_cast<int>(node);
}

Node* DOMNodeIds::nodeForId(int id)
{
    return reinterpret_cast<Node *>(id);
}

} // namespace blink
