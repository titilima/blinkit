#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: qjs_bindings.h
// Description: Bindings for QuickJS
//      Author: Ziming Li
//     Created: 2022-03-04
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_QJS_BINDINGS_H
#define BLINKIT_QJS_BINDINGS_H

#include "third_party/quickjs/quickjs.h"

namespace blink {
class EventTarget;
}

namespace BlinKit {
namespace qjs {

namespace Strings {
extern const char AddEventListener[];
}

void AddConsole(JSContext *ctx, JSValue global);

JSValue CreateEventTargetPrototype(JSContext *ctx);
JSValue AddEventListenerImpl(JSContext *ctx, EventTarget *eventTarget, int argc, JSValueConst *argv);

#ifdef BLINKIT_CRAWLER_ENABLED
JSValue CreateWindowPrototypeForCrawler(JSContext *ctx);

JSValue CreateNodePrototypeForCrawler(JSContext *ctx, JSValue eventTargetPrototype);
JSValue CreateContainerNodePrototypeForCrawler(JSContext *ctx, JSValue nodePrototype);
JSValue CreateDocumentPrototypeForCrawler(JSContext *ctx, JSValue containerNodePrototype);
JSValue CreateElementPrototypeForCrawler(JSContext *ctx, JSValue containerNodePrototype);
#endif

#ifdef BLINKIT_UI_ENABLED
JSValue CreateWindowPrototypeForUI(JSContext *ctx);

JSValue CreateNodePrototypeForUI(JSContext *ctx, JSValue eventTargetPrototype);
JSValue CreateContainerNodePrototypeForUI(JSContext *ctx, JSValue nodePrototype);
JSValue CreateDocumentPrototypeForUI(JSContext *ctx, JSValue containerNodePrototype);
#endif

} // namespace qjs
} // namespace BlinKit

#endif // BLINKIT_QJS_BINDINGS_H
