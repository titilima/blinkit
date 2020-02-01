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
class PrototypeHelper;
} // namespace BlinKit

namespace blink {

class ScriptWrappable : public GarbageCollectedFinalized<ScriptWrappable>
{
    WTF_MAKE_NONCOPYABLE(ScriptWrappable);
public:
    virtual ~ScriptWrappable(void) = default;

    bool OwnedByContext(void) const { return m_ownedByContext; }
protected:
    ScriptWrappable(void) = default;
private:
    friend class BlinKit::PrototypeHelper;

    bool m_ownedByContext = false;
    void *m_contextObject = nullptr;
};

}  // namespace blink

#define DEFINE_WRAPPERTYPEINFO()        \
private:                                \
    typedef int ThisIsOnlyAPlaceholder

#endif  // BLINKIT_BLINK_SCRIPT_WRAPPABLE_H
