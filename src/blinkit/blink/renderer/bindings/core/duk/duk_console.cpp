// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_console.cpp
// Description: DukConsole Class
//      Author: Ziming Li
//     Created: 2020-02-29
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "./duk_console.h"

#include "bk_js.h"
#include "blinkit/blink/renderer/bindings/core/duk/script_controller.h"

using namespace blink;

namespace BlinKit {

static void DefaultOutput(int type, duk_context *ctx)
{
    const char *message = duk_to_string(ctx, 0);
    if (ScriptController *scriptController = ScriptController::From(ctx))
        scriptController->ConsoleOutput(type, duk_to_string(ctx, 0));
    else
        ContextImpl::DefaultConsoleOutput(type, message);
}

namespace Impl {

static duk_ret_t Log(duk_context *ctx)
{
    DefaultOutput(BK_CONSOLE_LOG, ctx);
    return 0;
}

static duk_ret_t Warn(duk_context *ctx)
{
    DefaultOutput(BK_CONSOLE_WARN, ctx);
    return 0;
}

} // namespace Impl

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char DukConsole::ProtoName[] = "Console";

void DukConsole::FillPrototypeEntry(PrototypeEntry &entry)
{
    static const PrototypeEntry::Method Methods[] = {
        { "log",  Impl::Log,  1 },
        { "warn", Impl::Warn, 1 }
    };

    DukScriptObject::FillPrototypeEntry(entry);
    entry.Add(Methods, std::size(Methods));
}

void DukConsole::RegisterPrototype(PrototypeHelper &helper)
{
    helper.Register(ProtoName, FillPrototypeEntry);
}

} // namespace BlinKit
