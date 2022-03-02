#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: script_wrappable.h
// Description: ScriptWrappable Class
//      Author: Ziming Li
//     Created: 2022-03-02
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_SCRIPT_WRAPPABLE_H
#define BLINKIT_SCRIPT_WRAPPABLE_H

#pragma once

#include "blinkit/blink/renderer/platform/heap/Handle.h"
#include "blinkit/blink/renderer/wtf/Noncopyable.h"
#include "third_party/quickjs/quickjs.h"

namespace blink {

class ScriptWrappable : public BlinKit::GCStub
{
    WTF_MAKE_NONCOPYABLE(ScriptWrappable);
public:
    virtual ~ScriptWrappable(void) = default;
protected:
    ScriptWrappable(void) = default;
private:
#ifndef NDEBUG
    BlinKit::GCObject* ObjectForGC(void) override
    {
        ASSERT_NOT_REACHED();
        return nullptr;
    }
#endif

    JSValue m_contextObject = 0;
};

} // namespace blink

#define DEFINE_WRAPPERTYPEINFO()        \
private:                                \
    typedef int ThisIsOnlyAPlaceholder

#endif // BLINKIT_SCRIPT_WRAPPABLE_H
