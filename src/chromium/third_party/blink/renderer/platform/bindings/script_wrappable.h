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

    bool IsContextRetained(void) const { return m_contextRetained; }
    void RetainByContext(void) { m_contextRetained = true; }
    void ReleaseFromContext(void) { m_contextRetained = false; }

    bool IsInGCPool(void) const { return m_inGCPool; }
    bool CanBePooled(void) const {
#ifdef _DEBUG
        return m_canBePooled;
#else
        return true;
#endif
    }

    virtual void PreCollectGarbage(BlinKit::GCPool &gcPool) {}
protected:
    ScriptWrappable(void)
        : m_contextRetained(false), m_inGCPool(false)
#ifdef _DEBUG
        , m_canBePooled(true)
#endif
    {
    }

    void SetCannotBePooled(void) {
#ifdef _DEBUG
        m_canBePooled = false;
#endif
    }
private:
    friend class BlinKit::DukScriptObject;
    friend class BlinKit::GCPool;
    friend class BlinKit::PushWrapper;

    bool m_contextRetained : 1;
    bool m_inGCPool : 1;
#ifdef _DEBUG
    bool m_canBePooled : 1;
#endif
    void *m_contextObject = nullptr;
};

} // namespace blink

#define DEFINE_WRAPPERTYPEINFO()        \
private:                                \
    typedef int ThisIsOnlyAPlaceholder

#endif // BLINKIT_BLINK_SCRIPT_WRAPPABLE_H
