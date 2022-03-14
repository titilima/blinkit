// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: qjs_location.cpp
// Description: Bindings for Location
//      Author: Ziming Li
//     Created: 2022-03-14
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#include "./qjs_bindings.h"

#include "blinkit/blink/renderer/bindings/core/qjs/qjs.h"
#include "blinkit/blink/renderer/bindings/core/qjs/qjs_c_function_list_entries.h"
#include "blinkit/blink/renderer/core/frame/Location.h"

namespace BlinKit {
namespace qjs {

static JSValue GetHref(JSContext *ctx, JSValueConst thisVal)
{
    Location *location = ScriptWrappable::Cast<Location>(thisVal);
    return ReturnString(ctx, location->href());
}

#ifdef BLINKIT_CRAWLER_ENABLED
namespace Crawler {

static JSValue SetHref(JSContext *ctx, JSValueConst thisVal, JSValueConst newHref)
{
    ASSERT(false); // BKTODO:
    return JS_UNDEFINED;
}

} // namespace Crawler

JSValue CreateLocationPrototypeForCrawler(JSContext *ctx)
{
    static const JSCFunctionListEntry Funcs[] = {
        CGetSetEntry("href", GetHref, Crawler::SetHref)
    };

    JSValue ret = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, ret, Funcs, std::size(Funcs));
    return ret;
}
#endif

#ifdef BLINKIT_UI_ENABLED
JSValue CreateLocationPrototypeForUI(JSContext *ctx)
{
    static const JSCFunctionListEntry Funcs[] = {
        CGetSetEntry("href", GetHref, nullptr)
    };

    JSValue ret = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, ret, Funcs, std::size(Funcs));
    return ret;
}
#endif

} // namespace qjs
} // namespace BlinKit
