// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_window.h
// Description: DukWindow Class
//      Author: Ziming Li
//     Created: 2020-01-30
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_DUK_WINDOW_H
#define BLINKIT_BLINK_DUK_WINDOW_H

#pragma once

#include "third_party/blink/renderer/core/frame/local_dom_window.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_event_target.h"

namespace BlinKit {

class PrototypeHelper;

class DukWindow final : public DukEventTarget
{
public:
    static void Attach(duk_context *ctx, blink::LocalDOMWindow &window);
    static void RegisterPrototypeForCrawler(PrototypeHelper &helper);

    static duk_ret_t SetTimerImpl(duk_context *ctx, bool repeatable);
private:
    static const char ProtoName[]; // Make this private to forbid the call of DukScriptObject::Create<DukWindow>

    static void FillPrototypeEntryForCrawler(PrototypeEntry &entry);
};

} // namespace BlinKit

#endif // BLINKIT_BLINK_DUK_WINDOW_H
