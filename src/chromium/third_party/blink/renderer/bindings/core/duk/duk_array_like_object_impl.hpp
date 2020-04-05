// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_array_like_object_impl.hpp
// Description: DukArrayLikeObjectImpl class
//      Author: Ziming Li
//     Created: 2020-04-01
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_DUK_ARRAY_LIKE_OBJECT_IMPL_HPP
#define BLINKIT_BLINK_DUK_ARRAY_LIKE_OBJECT_IMPL_HPP

#pragma once

#include "third_party/blink/renderer/bindings/core/duk/duk.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_script_object.h"
#include "third_party/blink/renderer/platform/bindings/script_wrappers.h"

namespace BlinKit {

template <class T, class NativeType>
class DukArrayLikeObjectImpl : public DukScriptObject
{
public:
    static duk_idx_t Push(duk_context *ctx, NativeType *nativeCollection)
    {
        PushWrapper w(ctx, nativeCollection);
        if (!DukScriptObject::Push(ctx, nativeCollection))
        {
            duk_push_bare_object(ctx);

            duk_push_c_function(ctx, DukScriptObject::DefaultFinalizer, 1);
            duk_set_finalizer(ctx, -2);

            DukScriptObject::BindNative(ctx, -1, *nativeCollection);

            T::InitializeProxy(ctx);
        }
        return duk_get_top_index(ctx);
    }
protected:
    static void InitializeProxy(duk_context *ctx)
    {
        duk_push_bare_object(ctx);
        duk_push_c_function(ctx, T::ProxyGet, 2);
        duk_put_prop_string(ctx, -2, "get");
        duk_push_proxy(ctx, 0);
    }
    static duk_ret_t ProxyGet(duk_context *ctx)
    {
        NativeType *nativeCollection = DukScriptObject::To<NativeType>(ctx, 0);

        duk_uarridx_t index;
        if (Duk::TryToArrayIndex(ctx, 1, index))
        {
            T::PushItem(ctx, nativeCollection, index);
            return 1;
        }

        if (!duk_is_string(ctx, 1))
        {
            NOTREACHED();
            return 0;
        }

        const std::string k = Duk::To<std::string>(ctx, 1);
        if (k == "length")
        {
            duk_push_uint(ctx, nativeCollection->length());
        }
        else
        {
#ifdef _DEBUG
            if (k == "nodeType") // Bypass jQuery.
                return 0;
#endif
            ASSERT(false); // BKTODO:
            return 0;
        }
        return 1;
    }
};

} // namespace BlinKit

#endif // BLINKIT_BLINK_DUK_ARRAY_LIKE_OBJECT_IMPL_HPP
