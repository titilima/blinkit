#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: GarbageCollected.h
// Description: GC Classes
//      Author: Ziming Li
//     Created: 2021-06-29
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_GARBAGE_COLLECTED_H
#define BLINKIT_BLINKIT_GARBAGE_COLLECTED_H

#include "blinkit/blink/renderer/platform/heap/Heap.h"
#include "blinkit/blink/renderer/platform/heap/persistent.h"
#include "blinkit/blink/renderer/wtf/TypeTraits.h"

namespace blink {

class GarbageCollectedMixin
{
    IS_GARBAGE_COLLECTED_TYPE();
};

template <typename T>
class GarbageCollectedFinalized : public GarbageCollected<T>
{
};

template<typename T>
class RefCountedGarbageCollected : public GarbageCollectedFinalized<T>
{
    WTF_MAKE_NONCOPYABLE(RefCountedGarbageCollected);
public:
    ~RefCountedGarbageCollected(void) = default;

    void ref(void)
    {
        if (UNLIKELY(0 == m_refCount))
            MakeKeepAlive();
        ++m_refCount;
    }
    void deref(void)
    {
        ASSERT(m_refCount > 0);
        if (0 == --m_refCount)
        {
            delete m_keepAlive;
            m_keepAlive = nullptr;
        }
    }

    bool hasOneRef(void) const
    {
        return m_refCount == 1;
    }
protected:
    RefCountedGarbageCollected(void) = default;
private:
    void MakeKeepAlive(void)
    {
        ASSERT(nullptr == m_keepAlive);
        m_keepAlive = new Persistent<T>(static_cast<T *>(this));
    }

    int m_refCount = 0;
    Persistent<T> *m_keepAlive = nullptr;
};

} // namespace blink

#define GC_PLUGIN_IGNORE(bug)
#define USING_GARBAGE_COLLECTED_MIXIN(type)
#define WILL_BE_USING_GARBAGE_COLLECTED_MIXIN(type)

#endif // BLINKIT_BLINKIT_GARBAGE_COLLECTED_H
