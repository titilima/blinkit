// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: qjs_window.cpp
// Description: Bindings for Window
//      Author: Ziming Li
//     Created: 2022-03-07
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#include "./qjs_bindings.h"

#include "blinkit/blink/renderer/bindings/core/qjs/qjs.h"
#include "blinkit/blink/renderer/bindings/core/qjs/qjs_c_function_list_entries.h"
#include "blinkit/blink/renderer/bindings/core/script_controller.h"
#include "blinkit/blink/renderer/core/dom/document.h"
#include "blinkit/blink/renderer/core/frame/local_dom_window.h"

namespace BlinKit {
namespace qjs {

namespace Strings {
static const char Document[] = "document";
static const char Location[] = "location";
static const char Window[] = "window";
}

static LocalDOMWindow* GetWindowFromContext(JSContext *ctx)
{
    return ScriptController::From(ctx)->GetWindow();
}

static JSValue AddEventListener(JSContext *ctx, JSValueConst, int argc, JSValueConst *argv)
{
    LocalDOMWindow *window = GetWindowFromContext(ctx);
    return AddEventListenerImpl(ctx, window, argc, argv);
}

static JSValue GetDocument(JSContext *ctx, JSValueConst)
{
    ScriptController *scriptController = ScriptController::From(ctx);

    LocalDOMWindow *window = scriptController->GetWindow();
    return scriptController->ReturnScriptWrappable(window->document());
}

static JSValue GetLocation(JSContext *ctx, JSValueConst)
{
    ScriptController *scriptController = ScriptController::From(ctx);

    LocalDOMWindow *window = scriptController->GetWindow();
    return scriptController->ReturnScriptWrappable(window->location());
}

static JSValue GetWindow(JSContext *ctx, JSValueConst thisVal)
{
    return JS_DupValue(ctx, thisVal);
}

#ifdef BLINKIT_CRAWLER_ENABLED
namespace Crawler {

static JSValue SetLocation(JSContext *ctx, JSValueConst thisVal, JSValueConst newLocation)
{
    ASSERT(false); // BKTODO:
    return JS_UNDEFINED;
}

} // namespace Crawler

JSValue CreateWindowPrototypeForCrawler(JSContext *ctx)
{
    static const JSCFunctionListEntry Funcs[] = {
        CFunctionEntry(Strings::AddEventListener, 2, AddEventListener),
        CGetSetEntry(Strings::Document, GetDocument, nullptr),
        CGetSetEntry(Strings::Location, GetLocation, Crawler::SetLocation),
        CGetSetEntry(Strings::Window, GetWindow, nullptr)
    };

    JSValue ret = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, ret, Funcs, std::size(Funcs));
    return ret;
}
#endif

#ifdef BLINKIT_UI_ENABLED
JSValue CreateWindowPrototypeForUI(JSContext *ctx)
{
    static const JSCFunctionListEntry Funcs[] = {
        CFunctionEntry(Strings::AddEventListener, 2, AddEventListener),
        CGetSetEntry(Strings::Document, GetDocument, nullptr),
        CGetSetEntry(Strings::Location, GetLocation, nullptr),
        CGetSetEntry(Strings::Window, GetWindow, nullptr)
    };

    JSValue ret = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, ret, Funcs, std::size(Funcs));
    return ret;
}
#endif

} // namespace qjs
} // namespace BlinKit
