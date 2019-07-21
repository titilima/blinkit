// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_css_style_declaration.h
// Description: Bindings for CSSStyleDeclaration
//      Author: Ziming Li
//     Created: 2019-07-20
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_JS_DUK_CSS_STYLE_DECLARATION_H
#define BLINKIT_JS_DUK_CSS_STYLE_DECLARATION_H

#pragma once

#include "core/css/CSSStyleDeclaration.h"
#include "duk_object_impl.hpp"

namespace BlinKit {

class PrototypeManager;

class DukCSSStyleDeclaration final : public DukObjectImpl<blink::CSSStyleDeclaration>
{
    friend class DukContext;
public:
    static const char ProtoName[];

    static void RegisterPrototype(duk_context *ctx, PrototypeManager &protos);
private:
    static duk_ret_t Finalizer(duk_context *ctx);
    static duk_ret_t ProxyGet(duk_context *ctx);
    static duk_ret_t ProxySet(duk_context *ctx);
    static void OnPush(duk_context *ctx, blink::ScriptWrappable *nativeThis);
};

} // namespace BlinKit

#endif // BLINKIT_JS_DUK_CSS_STYLE_DECLARATION_H
