// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_node_list.h
// Description: Bindings for NodeList
//      Author: Ziming Li
//     Created: 2019-06-04
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_JS_DUK_NODE_LIST_H
#define BLINKIT_JS_DUK_NODE_LIST_H

#pragma once

#include "core/dom/StaticNodeList.h"

namespace BlinKit {

class PrototypeEntry;
class PrototypeManager;

class DukNodeList
{
    friend class DukContext;
public:
    static const char ProtoName[];

    static void RegisterPrototype(duk_context *ctx, PrototypeManager &protos);
private:
    static duk_ret_t Finalizer(duk_context *ctx);
    static void OnCreate(duk_context *ctx, blink::ScriptWrappable *nativeThis);
};

} // namespace BlinKit

#endif // BLINKIT_JS_DUK_NODE_LIST_H
