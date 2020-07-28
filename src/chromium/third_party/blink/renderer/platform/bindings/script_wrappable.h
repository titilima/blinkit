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

#include <vector>
#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/wtf/noncopyable.h"

namespace BlinKit {
class DukScriptObject;
class GCPool;
class PushWrapper;
} // namespace BlinKit

namespace blink {

class ScriptWrappable : public GarbageCollectedFinalized<ScriptWrappable>
{
    WTF_MAKE_NONCOPYABLE(ScriptWrappable);
public:
    virtual ~ScriptWrappable(void) = default;

    enum GCType {
        GC_MANUAL = 0,
        GC_IN_FINALIZER,
        GC_IN_POOL
    };
    virtual GCType GetGCType(void) const = 0;

    bool IsRetainedByContext(void) const { return nullptr != m_contextObject; }

    bool IsMarkedForGC(void) const
    {
        ASSERT(GetGCType() != GC_MANUAL);
        return m_garbageFlag;
    }
    void SetGarbageFlag(void)
    {
        ASSERT(GetGCType() != GC_MANUAL);
        m_garbageFlag = true;
    }
    void ClearGarbageFlag(void)
    {
        ASSERT(GetGCType() != GC_MANUAL);
        m_garbageFlag = false;
    }

    virtual void GetChildrenForGC(std::vector<ScriptWrappable *> &dst) {}
protected:
    ScriptWrappable(void) = default;
private:
    friend class BlinKit::DukScriptObject;
    friend class BlinKit::PushWrapper;

    bool m_garbageFlag = false;
    void *m_contextObject = nullptr;
};

} // namespace blink

#define DEFINE_WRAPPERTYPEINFO()        \
private:                                \
    typedef int ThisIsOnlyAPlaceholder

#endif // BLINKIT_BLINK_SCRIPT_WRAPPABLE_H
