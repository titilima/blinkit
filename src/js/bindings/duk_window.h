// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_window.h
// Description: Bindings for Window
//      Author: Ziming Li
//     Created: 2019-05-19
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_JS_DUK_WINDOW_H
#define BLINKIT_JS_DUK_WINDOW_H

#pragma once

#include "core/frame/DOMWindow.h"

#include "duk_event_target.h"

namespace BlinKit {

class DukWindow final : public DukEventTarget
{
public:
    static const char ProtoName[];

    static void RegisterPrototypeForCrawler(duk_context *ctx, PrototypeManager &protos);
#ifndef BLINKIT_CRAWLER_ONLY
    static void RegisterPrototypeForUI(duk_context *ctx, PrototypeManager &protos);
#endif
};

} // namespace BlinKit

#endif // BLINKIT_JS_DUK_WINDOW_H
