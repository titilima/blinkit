// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_event.h
// Description: Bindings for Event
//      Author: Ziming Li
//     Created: 2019-07-26
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_JS_DUK_EVENT_H
#define BLINKIT_JS_DUK_EVENT_H

#pragma once

#include "core/events/Event.h"
#include "duk_object_impl.hpp"

namespace BlinKit {

class PrototypeEntry;
class PrototypeManager;

class DukEvent : public DukObjectImpl<blink::Event>
{
    friend class DukContext;
public:
    static const char ProtoName[];

    template <class T>
    static T* GetNativeThis(duk_context *ctx, duk_idx_t idx = -1) {
        return static_cast<T *>(Get(ctx, idx));
    }

    static void RegisterPrototype(duk_context *ctx, PrototypeManager &protos);
protected:
    static void RegisterToPrototypeEntry(PrototypeEntry &entry);
private:
    static blink::Event* Get(duk_context *ctx, duk_idx_t idx = -1);
    static duk_ret_t Finalizer(duk_context *ctx);
};

} // namespace BlinKit

#endif // BLINKIT_JS_DUK_EVENT_H
