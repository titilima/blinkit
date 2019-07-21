// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_css_style_declaration.cpp
// Description: Bindings for CSSStyleDeclaration
//      Author: Ziming Li
//     Created: 2019-07-20
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "duk_css_style_declaration.h"

#include "core/css/CSSValue.h"

#include "bindings/duk_exception_state.h"
#include "context/duk_context.h"
#include "context/prototype_manager.h"
#include "wrappers/duk.h"

using namespace blink;

namespace BlinKit {

const char DukCSSStyleDeclaration::ProtoName[] = "CSSStyleDeclaration";

static CSSStyleDeclaration* Get(duk_context *ctx, duk_idx_t idx = -1)
{
    return static_cast<CSSStyleDeclaration *>(Duk::GetNativeThis(ctx, idx));
}

static CSSPropertyID ParseCSSPropertyID(const char *name)
{
    std::string builder;

    if (0 == strncmp(name, "css", 3))
        name += 3;
    else if (0 == strncmp(name, "webkit", 6))
        builder.push_back('-');
    else if (isASCIIUpper(*name))
        return CSSPropertyInvalid;

    bool hasSeenDash = false;
    bool hasSeenUpper = isASCIIUpper(*name);
    builder.push_back(toASCIILower(*name));
    for (const char *p = name + 1; '\0' != *p; ++p)
    {
        char c = *p;
        if (!isASCIIUpper(c))
        {
            if ('-' == c)
                hasSeenDash = true;
            builder.push_back(c);
        }
        else
        {
            hasSeenUpper = true;
            builder.push_back('-');
            builder.push_back(toASCIILower(c));
        }
    }

    // Reject names containing both dashes and upper-case characters, such as "border-rightColor".
    if (hasSeenDash && hasSeenUpper)
        return CSSPropertyInvalid;

    String propName = String::fromUTF8(builder.data(), builder.length());
    return unresolvedCSSPropertyID(propName);
}

duk_ret_t DukCSSStyleDeclaration::Finalizer(duk_context *ctx)
{
    CSSStyleDeclaration *nativeThis = Get(ctx, 0);
    DukContext::From(ctx)->RemoveObjectFromPool(nativeThis);
    return 0;
}

void DukCSSStyleDeclaration::OnPush(duk_context *ctx, ScriptWrappable *)
{
    duk_push_object(ctx);
    duk_push_c_function(ctx, ProxyGet, 3);
    duk_put_prop_string(ctx, -2, "get");
    duk_push_c_function(ctx, ProxySet, 4);
    duk_put_prop_string(ctx, -2, "set");
    duk_push_proxy(ctx, 0);
}

namespace Impl {

static duk_ret_t CSSTextGetter(duk_context *ctx, CSSStyleDeclaration *style)
{
    Duk::PushString(ctx, style->cssText());
    return 1;
}

static duk_ret_t CSSTextSetter(duk_context *ctx, CSSStyleDeclaration *style, const String &val)
{
    DukExceptionState es(ctx, "cssText", DukCSSStyleDeclaration::ProtoName);
    style->setCSSText(val, es);
    if (es.hadException())
        es.throwIfNeeded();
    return 0;
}

} // namespace Impl

duk_ret_t DukCSSStyleDeclaration::ProxyGet(duk_context *ctx)
{
    CSSStyleDeclaration *style = Get(ctx, 0);

    const std::string key = Duk::ToString(ctx, 1);
    if (key == "cssText")
        return Impl::CSSTextGetter(ctx, style);

    CSSPropertyID unresolvedId = ParseCSSPropertyID(key.c_str());
    if (CSSPropertyInvalid == unresolvedId)
    {
        assert(false); // Invalid property ID!
        return 0;
    }

    String ret;
    CSSPropertyID resolvedId = resolveCSSPropertyID(unresolvedId);
    PassRefPtr<CSSValue> cssValue = style->getPropertyCSSValueInternal(resolvedId);
    if (cssValue)
        ret = cssValue->cssText();
    else
        ret = style->getPropertyValueInternal(resolvedId);
    Duk::PushString(ctx, ret);
    return 1;
}

duk_ret_t DukCSSStyleDeclaration::ProxySet(duk_context *ctx)
{
    CSSStyleDeclaration *style = Get(ctx, 0);

    const std::string key = Duk::ToString(ctx, 1);
    if (key == "cssText")
        return Impl::CSSTextSetter(ctx, style, Duk::ToWTFString(ctx, 2));

    CSSPropertyID propId = ParseCSSPropertyID(key.c_str());
    assert(false); // BKTODO:
    return 0;
}

void DukCSSStyleDeclaration::RegisterPrototype(duk_context *ctx, PrototypeManager &protos)
{
    const auto worker = [](PrototypeEntry &entry)
    {
        entry.SetFinalizer(Finalizer);
    };
    protos.Register(ctx, ProtoName, worker);
}

} // namespace BlinKit
