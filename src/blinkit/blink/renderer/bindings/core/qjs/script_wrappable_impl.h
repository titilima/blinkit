#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: script_wrappable_impl.h
// Description: ScriptWrappable Class
//      Author: Ziming Li
//     Created: 2022-03-02
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_SCRIPT_WRAPPABLE_IMPL_H
#define BLINKIT_SCRIPT_WRAPPABLE_IMPL_H

#include "blinkit/blink/renderer/platform/heap/Handle.h"
#include "blinkit/blink/renderer/wtf/Noncopyable.h"
#include "third_party/quickjs/quickjs.h"

namespace blink {

class ScriptWrappable
{
    WTF_MAKE_NONCOPYABLE(ScriptWrappable);
public:
    virtual ~ScriptWrappable(void);

    static void Initialize(void);

    template <class T>
    static T* Cast(JSValue v) {
        return static_cast<T *>(CastImpl(v));
    }
protected:
    ScriptWrappable(void);
private:
    static ScriptWrappable* CastImpl(JSValue v);
    static JSValue NewObject(JSContext *ctx, JSValueConst proto, ScriptWrappable *nativeObject);

    friend class ScriptController;
    JSValue m_scriptObject;
};

} // namespace blink

#endif // BLINKIT_SCRIPT_WRAPPABLE_IMPL_H
