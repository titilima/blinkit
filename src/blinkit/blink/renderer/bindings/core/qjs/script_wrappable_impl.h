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

    static void Initialize(JSRuntime *runtime);
protected:
    ScriptWrappable(void) = default;
private:
    JSValue m_contextObject = 0;
};

} // namespace blink

#endif // BLINKIT_SCRIPT_WRAPPABLE_IMPL_H
