// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_node_list.h
// Description: DukNodeList Class
//      Author: Ziming Li
//     Created: 2020-03-31
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_DUK_NODE_LIST_H
#define BLINKIT_BLINK_DUK_NODE_LIST_H

#pragma once

#include "third_party/blink/renderer/bindings/core/duk/duk_array_like_object_impl.hpp"
#include "third_party/blink/renderer/core/dom/node_list.h"

namespace BlinKit {

class DukNodeList final : public DukArrayLikeObjectImpl<DukNodeList, blink::NodeList>
{
private:
    template <class T, class N> friend class DukArrayLikeObjectImpl;
    static void PushItem(duk_context *ctx, blink::NodeList *nodeList, duk_uarridx_t index);
};

} // namespace BlinKit

#endif // BLINKIT_BLINK_DUK_NODE_LIST_H
