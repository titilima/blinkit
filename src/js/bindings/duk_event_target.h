// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_event_target.h
// Description: Bindings for EventTarget
//      Author: Ziming Li
//     Created: 2019-06-01
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_JS_DUK_EVENT_TARGET_H
#define BLINKIT_JS_DUK_EVENT_TARGET_H

#pragma once

#include "meta_data.h"

namespace blink {
class EventTarget;
class ScriptWrappable;
}

namespace BlinKit {

class PrototypeEntry;
class PrototypeManager;

class DukEventTarget
{
    friend class DukContext;
public:
    template <class T>
    static T* GetNativeThis(duk_context *ctx, duk_idx_t idx = -1) {
        return static_cast<T *>(Get(ctx, idx));
    }
protected:
    static void RegisterToPrototypeEntry(PrototypeEntry &entry);
private:
    static blink::EventTarget* Get(duk_context *ctx, duk_idx_t idx = -1);
    static duk_ret_t Finalizer(duk_context *ctx);
    static void OnCreate(blink::ScriptWrappable *nativeThis);
};

} // namespace BlinKit

#endif
