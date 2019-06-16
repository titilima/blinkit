// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_console.cpp
// Description: Bindings for Console
//      Author: Ziming Li
//     Created: 2019-05-20
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "duk_console.h"

#include "context/prototype_manager.h"
#include "wrappers/duk.h"

namespace BlinKit {

const char DukConsole::ProtoName[] = "Console";
MetaData DukConsole::ClassMetaData = { MetaData::Console, nullptr };

namespace Impl {

static duk_ret_t Log(duk_context *ctx)
{
    int idx = 0;
    if (!duk_is_string(ctx, 0))
    {
        duk_dup(ctx, 0);
        idx = -1;
    }

    const std::string log = Duk::ToString(ctx, idx);
    BKLOG("[LOG] %s", log.c_str());
    return 0;
}

static duk_ret_t Warn(duk_context *ctx)
{
    const std::string msg = Duk::ToString(ctx, 0);
    BKLOG("[WARNING] %s", msg.c_str());
    return 0;
}

} // namespace Impl

void DukConsole::RegisterPrototype(duk_context *ctx, PrototypeManager &protos)
{
    static const PrototypeEntry::Method Methods[] = {
        { "log",  Impl::Log,  1 }, // BKTODO: DUK_VARARGS
        { "warn", Impl::Warn, 1 }  // BKTODO: DUK_VARARGS
    };
    const auto worker = [](PrototypeEntry &entry)
    {
        entry.Add(Methods, WTF_ARRAY_LENGTH(Methods));
    };
    protos.Register(ctx, ProtoName, worker);
}

} // namespace BlinKit
