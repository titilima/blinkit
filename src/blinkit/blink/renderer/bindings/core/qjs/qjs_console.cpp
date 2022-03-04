// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: qjs_console.cpp
// Description: Bindings for Console
//      Author: Ziming Li
//     Created: 2022-03-04
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#include "./qjs_bindings.h"

#include "blinkit/blink/renderer/bindings/core/qjs/qjs_c_function_list_entries.h"

namespace BlinKit {
namespace qjs {

namespace Strings {
static const char Log[] = "log";
}

static JSValue Log(JSContext *ctx, JSValueConst, int argc, JSValueConst *argv)
{
    std::string log;

    for (int i = 0; i < argc; ++i)
    {
        JSValue v = JS_ToString(ctx, argv[i]);
        if (JS_IsException(v))
            return v;

        size_t l = 0;
        const char *ps = JS_ToCStringLen(ctx, &l, v);
        log.append(ps, l);
        log.push_back(' ');

        JS_FreeValue(ctx, v);
    }

    if (!log.empty())
        log.pop_back();
    zed::log("[LOG] {}", log);
    return JS_UNDEFINED;
}

void AddConsole(JSContext *ctx, JSValue global)
{
    static const JSCFunctionListEntry ConsoleFuncs[] = {
        CFunctionEntry(Strings::Log, 1, Log),
        AliasEntry("info", Strings::Log)
    };
    static const JSCFunctionListEntry GlobalFuncs[] = {
        ObjectEntry("console", ConsoleFuncs, std::size(ConsoleFuncs), JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE)
    };
    JS_SetPropertyFunctionList(ctx, global, GlobalFuncs, std::size(GlobalFuncs));
}

} // namespace qjs
} // namespace BlinKit
