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

namespace BlinKit {

const char DukWindow::ProtoName[] = "Window";
MetaData DukWindow::ClassMetaData = { MetaData::Window, nullptr };

namespace Impl {

static duk_ret_t ConsoleGetter(duk_context *ctx)
{
    DukContext::From(ctx)->CreateObject(DukConsole::ProtoName);
    return 1;
}

static duk_ret_t ConsoleSetter(duk_context *ctx)
{
    assert(false); // Not reached!
    return 0;
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
        { "console", Impl::ConsoleGetter, Impl::ConsoleSetter }
    };
    const auto worker = [](PrototypeEntry &entry)
    {
        entry.Add(Properties, WTF_ARRAY_LENGTH(Properties));
    };
    protos.Register(ctx, ProtoName, worker);
}

} // namespace BlinKit
