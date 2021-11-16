// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_dom_token_list.cpp
// Description: DukDOMTokenList Class
//      Author: Ziming Li
//     Created: 2021-11-16
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./duk_dom_token_list.h"

#include <optional>
#include "blinkit/blink/renderer/bindings/core/duk/duk.h"
#include "blinkit/blink/renderer/bindings/core/duk/script_wrappers.h"

using namespace blink;

namespace BlinKit {

const char DukDOMTokenList::ProtoName[] = "DOMTokenList";

namespace Impl {

static Vector<String> ConvertStrings(const std::vector<std::string> &src)
{
    Vector<String> ret;
    for (const std::string &s : src)
        ret.append(String::fromStdUTF8(s));
    return ret;
}

static duk_ret_t Add(duk_context *ctx)
{
    std::vector<std::string> classNames;
    Duk::CollectStringArgs(ctx, classNames);

    duk_push_this(ctx);
    DOMTokenList *tokenList = DukScriptObject::To<DOMTokenList>(ctx, -1);

    DukExceptionState exceptionState(ctx);
    tokenList->add(ConvertStrings(classNames), exceptionState);
    if (exceptionState.hadException())
        exceptionState.ThrowIfNeeded();

    return 0;
}

static duk_ret_t Length(duk_context *ctx)
{
    duk_push_this(ctx);
    DOMTokenList *tokenList = DukScriptObject::To<DOMTokenList>(ctx, -1);
    duk_push_uint(ctx, tokenList->length());
    return 1;
}

static duk_ret_t Remove(duk_context *ctx)
{
    std::vector<std::string> classNames;
    Duk::CollectStringArgs(ctx, classNames);

    duk_push_this(ctx);
    DOMTokenList *tokenList = DukScriptObject::To<DOMTokenList>(ctx, -1);

    DukExceptionState exceptionState(ctx);
    tokenList->remove(ConvertStrings(classNames), exceptionState);
    if (exceptionState.hadException())
        exceptionState.ThrowIfNeeded();

    return 0;
}

static duk_ret_t Toggle(duk_context *ctx)
{
    const AtomicString className = Duk::To<AtomicString>(ctx, 0);

    std::optional<bool> force;
    if (duk_is_boolean(ctx, 1))
        force = duk_to_boolean(ctx, 1);

    duk_push_this(ctx);
    DOMTokenList *tokenList = DukScriptObject::To<DOMTokenList>(ctx, -1);

    bool ret;
    DukExceptionState exceptionState(ctx);
    if (force.has_value())
        ret = tokenList->toggle(className, force.value(), exceptionState);
    else
        ret = tokenList->toggle(className, exceptionState);

    if (exceptionState.hadException())
    {
        exceptionState.ThrowIfNeeded();
        return 0;
    }
    duk_push_boolean(ctx, ret);
    return 1;
}

} // namespace Impl

void DukDOMTokenList::FillPrototypeEntry(PrototypeEntry &entry)
{
    static const PrototypeEntry::Method Methods[] = {
        { "add",    Impl::Add,    DUK_VARARGS },
        { "remove", Impl::Remove, DUK_VARARGS },
        { "toggle", Impl::Toggle, 2           }
    };
    static const PrototypeEntry::Property Properties[] = {
        { "length", Impl::Length, nullptr }
    };

    DukScriptObject::FillPrototypeEntry(entry);
    entry.Add(Methods, std::size(Methods));
    entry.Add(Properties, std::size(Properties));
}

duk_idx_t DukDOMTokenList::Push(duk_context *ctx, DOMTokenList &tokenList)
{
    PushWrapper w(ctx, &tokenList);
    do {
        if (DukScriptObject::Push(ctx, &tokenList))
            break;

        PrototypeHelper::CreateScriptObject(ctx, ProtoName, &tokenList);
    } while (false);
    return duk_get_top_index(ctx);
}

void DukDOMTokenList::RegisterPrototype(PrototypeHelper &helper)
{
    helper.Register(ProtoName, FillPrototypeEntry);
}

} // namespace BlinKit
