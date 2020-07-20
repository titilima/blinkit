// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_location.cpp
// Description: DukLocation Class
//      Author: Ziming Li
//     Created: 2020-03-26
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "duk_location.h"

#include "third_party/blink/renderer/bindings/core/duk/duk.h"

using namespace blink;

namespace BlinKit {

const char DukLocation::ProtoName[] = "Location";

namespace Impl {

static duk_ret_t HashGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Location *location = DukScriptObject::To<Location>(ctx, 0);
    Duk::PushString(ctx, location->hash());
    return 1;
}

static duk_ret_t HashSetter(duk_context *ctx)
{
    ASSERT(false); // BKTODO:
    return 0;
}

static duk_ret_t HostGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Location *location = DukScriptObject::To<Location>(ctx, 0);
    Duk::PushString(ctx, location->host());
    return 1;
}

static duk_ret_t HostSetter(duk_context *ctx)
{
    ASSERT(false); // BKTODO:
    return 0;
}

static duk_ret_t HrefGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Location *location = DukScriptObject::To<Location>(ctx, 0);
    Duk::PushString(ctx, location->href());
    return 1;
}

static duk_ret_t HrefSetter(duk_context *ctx)
{
    ASSERT(false); // BKTODO:
    return 0;
}

static duk_ret_t PathnameGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Location *location = DukScriptObject::To<Location>(ctx, 0);
    Duk::PushString(ctx, location->pathname());
    return 1;
}

static duk_ret_t PathnameSetter(duk_context *ctx)
{
    ASSERT(false); // BKTODO:
    return 0;
}

static duk_ret_t ProtocolGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Location *location = DukScriptObject::To<Location>(ctx, 0);
    Duk::PushString(ctx, location->protocol());
    return 1;
}

static duk_ret_t ProtocolSetter(duk_context *ctx)
{
    ASSERT(false); // BKTODO:
    return 0;
}

static duk_ret_t SearchGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Location *location = DukScriptObject::To<Location>(ctx, 0);
    Duk::PushString(ctx, location->search());
    return 1;
}

static duk_ret_t SearchSetter(duk_context *ctx)
{
    ASSERT(false); // BKTODO:
    return 0;
}

} // namespace Impl

void DukLocation::FillPrototypeEntry(PrototypeEntry &entry)
{
    static const PrototypeEntry::Property Properties[] = {
        { "hash",     Impl::HashGetter,     Impl::HashSetter     },
        { "host",     Impl::HostGetter,     Impl::HostSetter     },
        { "href",     Impl::HrefGetter,     Impl::HrefSetter     },
        { "pathname", Impl::PathnameGetter, Impl::PathnameSetter },
        { "protocol", Impl::ProtocolGetter, Impl::ProtocolSetter },
        { "search",   Impl::SearchGetter,   Impl::SearchSetter     },
    };

    DukScriptObject::FillPrototypeEntry(entry);
    entry.Add(Properties, std::size(Properties));
}

void DukLocation::RegisterPrototype(PrototypeHelper &helper)
{
    helper.Register(ProtoName, FillPrototypeEntry);
}

} // namespace BlinKit
