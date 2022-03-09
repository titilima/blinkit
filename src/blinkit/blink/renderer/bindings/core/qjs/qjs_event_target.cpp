// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: qjs_event_target.cpp
// Description: Bindings for EventTarget
//      Author: Ziming Li
//     Created: 2022-03-07
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#include "./qjs_bindings.h"

#include "blinkit/blink/renderer/bindings/core/qjs/qjs.h"
#include "blinkit/blink/renderer/bindings/core/qjs/qjs_c_function_list_entries.h"

namespace BlinKit {
namespace qjs {

namespace Strings {
const char AddEventListener[] = "addEventListener";
}

JSValue AddEventListenerImpl(JSContext *ctx, EventTarget *eventTarget, int argc, JSValueConst *argv)
{
    ASSERT(false); // BKTODO:
    return JS_UNDEFINED;
}

static JSValue AddEventListener(JSContext *ctx, JSValueConst thisVal, int argc, JSValueConst *argv)
{
    ASSERT(false); // BKTODO:
    return JS_UNDEFINED;
}

JSValue CreateEventTargetPrototype(JSContext *ctx)
{
    static const JSCFunctionListEntry Funcs[] = {
        CFunctionEntry(Strings::AddEventListener, 2, AddEventListener)
    };

    JSValue ret = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, ret, Funcs, std::size(Funcs));
    return ret;
}

} // namespace qjs
} // namespace BlinKit
