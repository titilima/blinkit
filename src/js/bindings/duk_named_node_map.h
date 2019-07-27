// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_named_node_map.h
// Description: Bindings for NamedNodeMap
//      Author: Ziming Li
//     Created: 2019-07-22
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_JS_DUK_NAMED_NODE_MAP_H
#define BLINKIT_JS_DUK_NAMED_NODE_MAP_H

#pragma once

#include "core/dom/NamedNodeMap.h"
#include "duk_object_impl.hpp"

namespace BlinKit {

class PrototypeManager;

class DukNamedNodeMap final : public DukObjectImpl<blink::NamedNodeMap>
{
    friend class DukContext;
public:
    static const char ProtoName[];

    static void RegisterPrototype(duk_context *ctx, PrototypeManager &protos);
private:
    static duk_ret_t Finalizer(duk_context *ctx);
    static duk_ret_t ProxyGet(duk_context *ctx);
    static void OnPush(duk_context *ctx, blink::ScriptWrappable *nativeThis);
};

} // namespace BlinKit

#endif // BLINKIT_JS_DUK_NAMED_NODE_MAP_H
