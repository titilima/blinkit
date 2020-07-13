// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_script_object.h
// Description: DukScriptObject Class
//      Author: Ziming Li
//     Created: 2020-01-31
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_DUK_SCRIPT_OBJECT_H
#define BLINKIT_BLINK_DUK_SCRIPT_OBJECT_H

#pragma once

#include "third_party/blink/renderer/bindings/core/duk/duk_exception_state.h"
#include "third_party/blink/renderer/bindings/core/duk/prototype_helper.h"
#include "third_party/blink/renderer/platform/wtf/allocator.h"

namespace BlinKit {

class DukScriptObject
{
public:
    template <class T>
    static inline duk_idx_t Create(duk_context *ctx)
    {
        return PrototypeHelper::CreateScriptObject(ctx, T::ProtoName, nullptr);
    }
    template <class T>
    static inline duk_idx_t Create(duk_context *ctx, blink::ScriptWrappable &nativeObject)
    {
        return PrototypeHelper::CreateScriptObject(ctx, T::ProtoName, &nativeObject);
    }

    template <class T>
    static inline T* To(duk_context *ctx, duk_idx_t idx)
    {
        blink::ScriptWrappable *nativeObject = ToScriptWrappable(ctx, idx);
        return static_cast<T *>(nativeObject);
    }

    template <class T>
    static inline duk_idx_t Push(duk_context *ctx, blink::ScriptWrappable *nativeObject)
    {
        return PushScriptWrappable(ctx, T::ProtoName, nativeObject);
    }
protected:
    DukScriptObject(void) = default;

    static void BindNative(duk_context *ctx, duk_idx_t idx, blink::ScriptWrappable &nativeObject);
    static duk_ret_t DefaultFinalizer(duk_context *ctx);
    static void FillPrototypeEntry(PrototypeEntry &entry);
    static bool Push(duk_context *ctx, blink::ScriptWrappable *nativeObject);
private:
    friend class PrototypeHelper;
    static blink::ScriptWrappable* ToScriptWrappable(duk_context *ctx, duk_idx_t idx);
    static duk_idx_t PushScriptWrappable(duk_context *ctx, const char *protoName, blink::ScriptWrappable *nativeObject);
};

} // namespace BlinKit

#endif // BLINKIT_BLINK_DUK_SCRIPT_OBJECT_H
