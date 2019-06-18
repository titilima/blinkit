// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_xhr.cpp
// Description: Bindings for XHR
//      Author: Ziming Li
//     Created: 2019-06-18
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "duk_xhr.h"

#include "context/duk_context.h"
#include "context/prototype_manager.h"
#include "wrappers/duk.h"

namespace BlinKit {

const char DukXHR::ProtoName[] = "XMLHttpRequest";

duk_ret_t DukXHR::Constructor(duk_context *ctx)
{
    if (duk_is_constructor_call(ctx))
    {
        duk_push_this(ctx);
        DukContext::PrototypeManagerFrom(ctx)->Bind(ctx, ProtoName);
        return 0;
    }
    else
    {
        assert(false); // BKTODO:
        return 1;
    }
}

namespace Impl {

static duk_ret_t Abort(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 0;
}

static duk_ret_t GetAllResponseHeaders(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t GetResponseHeader(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t Open(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 0;
}

static duk_ret_t ReadyStateGetter(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t ResponseTextGetter(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t ResponseXMLGetter(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t Send(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 0;
}

static duk_ret_t SetRequestHeader(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 0;
}

static duk_ret_t StatusGetter(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t StatusTextGetter(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

} // namespace Impl

void DukXHR::RegisterPrototype(duk_context *ctx, PrototypeManager &protos)
{
    static const PrototypeEntry::Property Properties[] = {
        { "readyState",   Impl::ReadyStateGetter,   nullptr },
        { "responseText", Impl::ResponseTextGetter, nullptr },
        { "responseXML",  Impl::ResponseXMLGetter,  nullptr },
        { "status",       Impl::StatusGetter,       nullptr },
        { "statusText",   Impl::StatusTextGetter,   nullptr },
    };
    static const PrototypeEntry::Method Methods[] = {
        { "abort",                 Impl::Abort,                 0           },
        { "getAllResponseHeaders", Impl::GetAllResponseHeaders, 0           },
        { "getResponseHeader",     Impl::GetResponseHeader,     1           },
        { "open",                  Impl::Open,                  DUK_VARARGS },
        { "send",                  Impl::Send,                  1           },
        { "setRequestHeader",      Impl::SetRequestHeader,      2           },
    };
    const auto worker = [](PrototypeEntry &entry)
    {
        entry.Add(Properties, WTF_ARRAY_LENGTH(Properties));
        entry.Add(Methods, WTF_ARRAY_LENGTH(Methods));
    };
    protos.Register(ctx, ProtoName, worker);
}

} // namespace BlinKit
