// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_navigator.cpp
// Description: Bindings for Navigator
//      Author: Ziming Li
//     Created: 2019-08-31
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "duk_navigator.h"

#include "context/duk_context.h"
#include "context/prototype_manager.h"
#include "wrappers/duk.h"

using namespace blink;

namespace BlinKit {

const char DukNavigator::ProtoName[] = "Navigator";

static Navigator* Get(duk_context *ctx, duk_idx_t idx = -1)
{
    return static_cast<Navigator *>(Duk::GetNativeThis(ctx, idx));
}

duk_ret_t DukNavigator::Finalizer(duk_context *ctx)
{
    Navigator *nativeThis = Get(ctx, 0);
    DukContext::From(ctx)->RemoveObjectFromPool(nativeThis);
    return 0;
}

namespace Impl {

static duk_ret_t AppCodeNameGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Navigator *navigator = Get(ctx);
    return Duk::PushString(ctx, navigator->appCodeName());
}

static duk_ret_t AppNameGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Navigator *navigator = Get(ctx);
    return Duk::PushString(ctx, navigator->appName());
}

static duk_ret_t AppVersionGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Navigator *navigator = Get(ctx);
    return Duk::PushString(ctx, navigator->appVersion());
}

static duk_ret_t CookieEnabledGetter(duk_context *ctx)
{
    duk_push_true(ctx);
    return 1;
}

static duk_ret_t PlatformGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Navigator *navigator = Get(ctx);
    return Duk::PushString(ctx, navigator->platform());
}

static duk_ret_t UserAgentGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Navigator *navigator = Get(ctx);
    return Duk::PushString(ctx, navigator->userAgent());
}

} // namespace Impl

void DukNavigator::RegisterPrototype(duk_context *ctx, PrototypeManager &protos)
{
    static const PrototypeEntry::Property Properties[] = {
        { "appCodeName",   Impl::AppCodeNameGetter,   nullptr },
        { "appName",       Impl::AppNameGetter,       nullptr },
        { "appVersion",    Impl::AppVersionGetter,    nullptr },
        { "cookieEnabled", Impl::CookieEnabledGetter, nullptr },
        { "platform",      Impl::PlatformGetter,      nullptr },
        { "userAgent",     Impl::UserAgentGetter,     nullptr },
    };
    const auto worker = [](PrototypeEntry &entry)
    {
        entry.SetFinalizer(Finalizer);
        entry.Add(Properties, WTF_ARRAY_LENGTH(Properties));
    };
    protos.Register(ctx, ProtoName, worker);
}

} // namespace BlinKit
