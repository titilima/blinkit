// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_location.cpp
// Description: Bindings for Location
//      Author: Ziming Li
//     Created: 2019-06-11
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "duk_location.h"

#include "context/duk_context.h"
#include "context/prototype_manager.h"
#include "wrappers/duk.h"

using namespace blink;

namespace BlinKit {

const char DukLocation::ProtoName[] = "Location";

static Location* Get(duk_context *ctx, duk_idx_t idx = -1)
{
    return reinterpret_cast<Location *>(Duk::GetNativeThis(ctx, idx));
}

duk_ret_t DukLocation::Finalizer(duk_context *ctx)
{
    Location *nativeThis = Get(ctx, 0);
    DukContext::From(ctx)->RemoveObjectFromPool(nativeThis);
    nativeThis->deref();
    return 0;
}

void DukLocation::OnCreate(duk_context *, ScriptWrappable *nativeThis)
{
    reinterpret_cast<Location *>(nativeThis)->ref();
}

namespace Crawler {

static duk_ret_t Assign(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 0;
}

static duk_ret_t HashGetter(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t HashSetter(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 0;
}

static duk_ret_t HostGetter(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t HostSetter(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 0;
}

static duk_ret_t HostnameGetter(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t HostnameSetter(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 0;
}

static duk_ret_t HrefGetter(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t HrefSetter(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 0;
}

static duk_ret_t PathnameGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    Location *location = Get(ctx);
    return Duk::PushString(ctx, location->pathname());
}

static duk_ret_t PathnameSetter(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 0;
}

static duk_ret_t PortGetter(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t PortSetter(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 0;
}

static duk_ret_t ProtocolGetter(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t ProtocolSetter(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 0;
}

static duk_ret_t Reload(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 0;
}

static duk_ret_t Replace(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 0;
}

static duk_ret_t SearchGetter(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t SearchSetter(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 0;
}

} // namespace Crawler

void DukLocation::RegisterPrototypeForCrawler(duk_context *ctx, PrototypeManager &protos)
{
    static const PrototypeEntry::Property Properties[] = {
        { "hash",     Crawler::HashGetter,     Crawler::HashSetter     },
        { "host",     Crawler::HostGetter,     Crawler::HostSetter     },
        { "hostname", Crawler::HostnameGetter, Crawler::HostnameSetter },
        { "href",     Crawler::HrefGetter,     Crawler::HrefSetter     },
        { "pathname", Crawler::PathnameGetter, Crawler::PathnameSetter },
        { "port",     Crawler::PortGetter,     Crawler::PortSetter     },
        { "protocol", Crawler::ProtocolGetter, Crawler::ProtocolSetter },
        { "search",   Crawler::SearchGetter,   Crawler::SearchSetter   },
    };
    static const PrototypeEntry::Method Methods[] = {
        { "assign",  Crawler::Assign,  1 },
        { "reload",  Crawler::Reload,  1 },
        { "replace", Crawler::Replace, 1 },
    };
    const auto worker = [](PrototypeEntry &entry)
    {
        entry.Add(Properties, WTF_ARRAY_LENGTH(Properties));
        entry.Add(Methods, WTF_ARRAY_LENGTH(Methods));
    };
    protos.Register(ctx, ProtoName, worker);
}

} // namespace BlinKit
