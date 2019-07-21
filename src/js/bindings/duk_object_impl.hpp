// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_object_impl.hpp
// Description: Base Class for Binding Classes
//      Author: Ziming Li
//     Created: 2019-06-12
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_JS_DUK_OBJECT_IMPL_HPP
#define BLINKIT_JS_DUK_OBJECT_IMPL_HPP

#pragma once

namespace blink {
class ScriptWrappable;
}

namespace BlinKit {

template <class NativeType>
class DukObjectImpl
{
protected:
    static void OnCreate(duk_context *, blink::ScriptWrappable *nativeThis) {
        static_cast<NativeType *>(nativeThis)->ref();
    }
    static void OnPush(duk_context *, blink::ScriptWrappable *) {
        // Nothing to do by default.
        // Create proxies here if needed.
    }
    static void OnGC(blink::ScriptWrappable *nativeThis) {
        static_cast<NativeType *>(nativeThis)->deref();
    }
};

} // namespace BlinKit

#endif // BLINKIT_JS_DUK_OBJECT_IMPL_HPP
