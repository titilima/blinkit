// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_xhr.h
// Description: Bindings for XHR
//      Author: Ziming Li
//     Created: 2019-06-18
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_JS_DUK_XHR_H
#define BLINKIT_JS_DUK_XHR_H

#pragma once

namespace BlinKit {

class PrototypeManager;

class DukXHR final
{
public:
    static const char ProtoName[];

    static duk_ret_t Constructor(duk_context *ctx);

    static void RegisterPrototype(duk_context *ctx, PrototypeManager &protos);
};

} // namespace BlinKit

#endif // BLINKIT_JS_DUK_XHR_H
