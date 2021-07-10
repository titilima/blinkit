// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_node.h
// Description: DukNode Class
//      Author: Ziming Li
//     Created: 2020-02-07
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_DUK_NODE_H
#define BLINKIT_BLINK_DUK_NODE_H

#pragma once

#include "blinkit/blink/renderer/bindings/core/duk/duk_event_target.h"
#include "blinkit/blink/renderer/core/dom/Node.h"

namespace BlinKit {

namespace ProtoNames {
extern const char Comment[];
extern const char DocumentFragment[];
extern const char DocumentType[];
extern const char Text[];
}

class DukNode : public DukEventTarget
{
public:
    static void RegisterPrototype(PrototypeHelper &helper, const char *name);

    static duk_idx_t Push(duk_context *ctx, blink::Node *node);
protected:
    static void FillPrototypeEntry(PrototypeEntry &entry);
    static duk_ret_t TextContentGetter(duk_context *ctx);
    static duk_ret_t TextContentSetter(duk_context *ctx);
};

} // namespace BlinKit

#endif // BLINKIT_BLINK_DUK_NODE_H
