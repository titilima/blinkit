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

#include "duk_console.h"

#include "bk_js.h"
#include "blinkit/js/context_impl.h"

namespace BlinKit {

namespace Impl {

static duk_ret_t Log(duk_context *ctx)
{
    ContextImpl::From(ctx)->ConsoleOutput(BK_CONSOLE_LOG, duk_to_string(ctx, 0));
    return 0;
}

} // namespace Impl

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char DukConsole::ProtoName[] = "Console";

void DukConsole::FillPrototypeEntry(PrototypeEntry &entry)
{
    static const PrototypeEntry::Method Methods[] = {
        { "log", Impl::Log, 1 },
    };

    DukScriptObject::FillPrototypeEntry(entry);
    entry.Add(Methods, std::size(Methods));
}

void DukConsole::RegisterPrototype(PrototypeHelper &helper)
{
    helper.Register(ProtoName, FillPrototypeEntry);
}

} // namespace BlinKit
