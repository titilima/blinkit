// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: script_wrappable.h
// Description: ScriptWrappable Class
//      Author: Ziming Li
//     Created: 2019-09-16
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_SCRIPT_WRAPPABLE_H
#define BLINKIT_BLINK_SCRIPT_WRAPPABLE_H

#pragma once

#include "blinkit/blink/renderer/platform/heap/Handle.h"
#include "blinkit/blink/renderer/wtf/Noncopyable.h"

namespace BlinKit {
class DukScriptObject;
class PushWrapper;
} // namespace BlinKit

namespace blink {

class ScriptWrappable : public BlinKit::GCStub
{
    WTF_MAKE_NONCOPYABLE(ScriptWrappable);
public:
    virtual ~ScriptWrappable(void) = default;
protected:
    ScriptWrappable(void) = default;
private:
    friend class BlinKit::DukScriptObject;
    friend class BlinKit::PushWrapper;

#ifndef NDEBUG
    BlinKit::GCObject* ObjectForGC(void) override
    {
        ASSERT_NOT_REACHED();
        return nullptr;
    }
#endif

    void *m_contextObject = nullptr;
};

} // namespace blink

#define DEFINE_WRAPPERTYPEINFO()        \
private:                                \
    typedef int ThisIsOnlyAPlaceholder

#endif // BLINKIT_BLINK_SCRIPT_WRAPPABLE_H
