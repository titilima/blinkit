// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_window.cpp
// Description: Bindings for Window
//      Author: Ziming Li
//     Created: 2019-05-19
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "duk_window.h"

#include "bindings/duk_console.h"
#include "context/duk_context.h"
#include "context/prototype_manager.h"
#include "wrappers/duk.h"

namespace BlinKit {

const char DukWindow::ProtoName[] = "Window";
MetaData DukWindow::ClassMetaData = { MetaData::Window, nullptr };

namespace Crawler {

} // namespace Crawler

namespace Impl {

static duk_ret_t ClosedGetter(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t ConsoleGetter(duk_context *ctx)
{
    DukContext::From(ctx)->CreateObject(DukConsole::ProtoName);
    return 1;
}

static duk_ret_t ConsoleSetter(duk_context *ctx)
{
    return Duk::DefaultSetter(ctx, "console");
}

static duk_ret_t DocumentGetter(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t FramesGetter(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t FramesSetter(duk_context *ctx)
{
    return Duk::DefaultSetter(ctx, "frames");
}

static duk_ret_t LocationGetter(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t LocationSetter(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 0;
}

static duk_ret_t NavigatorGetter(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t ParentSetter(duk_context *ctx)
{
    return Duk::DefaultSetter(ctx, "parent");
}

static duk_ret_t WindowGetter(duk_context *ctx)
{
    duk_push_global_object(ctx);
    return 1;
}

} // namespace Impl

#ifndef BLINKIT_CRAWLER_ONLY
void DukWindow::RegisterPrototype(duk_context *ctx, PrototypeManager &protos)
{
    static const PrototypeEntry::Property Properties[] = {
        { "console", Impl::ConsoleGetter, Impl::ConsoleSetter }
    };
    const auto worker = [](PrototypeEntry & entry)
    {
        entry.Add(Properties, WTF_ARRAY_LENGTH(Properties));
    };
    protos.Register(ctx, ProtoName, worker);
}
#endif // BLINKIT_CRAWLER_ONLY

void DukWindow::RegisterPrototypeForCrawler(duk_context *ctx, PrototypeManager &protos)
{
    static const PrototypeEntry::Property Properties[] = {
        { "closed",    Impl::ClosedGetter,      nullptr                 },
        { "console",   Impl::ConsoleGetter,     Impl::ConsoleSetter     },
        { "document",  Impl::DocumentGetter,    nullptr                 },
        { "frames",    Impl::FramesGetter,      Impl::FramesSetter      },
        { "location",  Impl::LocationGetter,    Impl::LocationSetter    },
        { "navigator", Impl::NavigatorGetter,   nullptr                 },
        { "parent",    Impl::WindowGetter,      Impl::ParentSetter      },
        { "self",      Impl::WindowGetter,      nullptr                 },
        { "top",       Impl::WindowGetter,      nullptr                 },
        { "window",    Impl::WindowGetter,      nullptr                 },
    };
    const auto worker = [](PrototypeEntry &entry)
    {
        entry.Add(Properties, WTF_ARRAY_LENGTH(Properties));
        entry.AddObject("external");
    };
    protos.Register(ctx, ProtoName, worker);
}

} // namespace BlinKit
