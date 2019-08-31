// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_navigator.h
// Description: Bindings for Navigator
//      Author: Ziming Li
//     Created: 2019-08-31
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_JS_DUK_NAVIGATOR_H
#define BLINKIT_JS_DUK_NAVIGATOR_H

#pragma once

#include "core/frame/Navigator.h"
#include "duk_object_impl.hpp"

namespace BlinKit {

class PrototypeManager;

class DukNavigator final : public DukObjectImpl<blink::Navigator>
{
    friend class DukContext;
public:
    static const char ProtoName[];

    static void RegisterPrototype(duk_context *ctx, PrototypeManager &protos);
private:
    static duk_ret_t Finalizer(duk_context *ctx);
    static void OnCreate(duk_context *, blink::ScriptWrappable *) {}
    static void OnGC(blink::ScriptWrappable *) {}
};

} // namespace BlinKit

#endif // BLINKIT_JS_DUK_NAVIGATOR_H
