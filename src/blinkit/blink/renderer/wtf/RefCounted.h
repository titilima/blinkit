// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: RefCounted.h
// Description: RefCounted Class
//      Author: Ziming Li
//     Created: 2021-06-24
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2006, 2007, 2008, 2009, 2010 Apple Inc. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

#ifndef RefCounted_h
#define RefCounted_h

#include "wtf/Allocator.h"
#include "wtf/Assertions.h"
// BKTODO: #include "wtf/InstanceCounter.h"
#include "wtf/Noncopyable.h"
#include "wtf/WTFExport.h"

#if ENABLE(ASSERT)
#define CHECK_REF_COUNTED_LIFECYCLE 1
#include "wtf/ThreadRestrictionVerifier.h"
#else
#define CHECK_REF_COUNTED_LIFECYCLE 0
#endif

namespace WTF {

// This base class holds the non-template methods and attributes.
// The RefCounted class inherits from it reducing the template bloat
// generated by the compiler (technique called template hoisting).
class WTF_EXPORT RefCountedBase {
public:
    void ref()
    {
#if CHECK_REF_COUNTED_LIFECYCLE
        // Start thread verification as soon as the ref count gets to 2. This
        // heuristic reflects the fact that items are often created on one thread
        // and then given to another thread to be used.
        // FIXME: Make this restriction tigher. Especially as we move to more
        // common methods for sharing items across threads like CrossThreadCopier.h
        // We should be able to add a "detachFromThread" method to make this explicit.
        if (m_refCount == 1)
            m_verifier.setShared(true);
        // If this assert fires, it either indicates a thread safety issue or
        // that the verification needs to change. See ThreadRestrictionVerifier for
        // the different modes.
        ASSERT(m_verifier.isSafeToUse());
        ASSERT(!m_adoptionIsRequired);
#endif
        ASSERT_WITH_SECURITY_IMPLICATION(!m_deletionHasBegun);
        ++m_refCount;
    }

    bool hasOneRef() const
    {
        ASSERT_WITH_SECURITY_IMPLICATION(!m_deletionHasBegun);
#if CHECK_REF_COUNTED_LIFECYCLE
        ASSERT(m_verifier.isSafeToUse());
#endif
        return m_refCount == 1;
    }

    int refCount() const
    {
#if CHECK_REF_COUNTED_LIFECYCLE
        ASSERT(m_verifier.isSafeToUse());
#endif
        return m_refCount;
    }

protected:
    RefCountedBase()
        : m_refCount(1)
#if ENABLE(SECURITY_ASSERT)
        , m_deletionHasBegun(false)
#endif
#if CHECK_REF_COUNTED_LIFECYCLE
        , m_adoptionIsRequired(true)
#endif
    {
    }

    ~RefCountedBase()
    {
        ASSERT_WITH_SECURITY_IMPLICATION(m_deletionHasBegun);
#if CHECK_REF_COUNTED_LIFECYCLE
        ASSERT(!m_adoptionIsRequired);
#endif
    }

    // Returns whether the pointer should be freed or not.
    bool derefBase()
    {
        ASSERT_WITH_SECURITY_IMPLICATION(!m_deletionHasBegun);
#if CHECK_REF_COUNTED_LIFECYCLE
        ASSERT(m_verifier.isSafeToUse());
        ASSERT(!m_adoptionIsRequired);
#endif

        ASSERT(m_refCount > 0);
        --m_refCount;
        if (!m_refCount) {
#if ENABLE(SECURITY_ASSERT)
            m_deletionHasBegun = true;
#endif
            return true;
        }

#if CHECK_REF_COUNTED_LIFECYCLE
        // Stop thread verification when the ref goes to 1 because it
        // is safe to be passed to another thread at this point.
        if (m_refCount == 1)
            m_verifier.setShared(false);
#endif
        return false;
    }

#if CHECK_REF_COUNTED_LIFECYCLE
    bool deletionHasBegun() const
    {
        return m_deletionHasBegun;
    }
#endif

private:

#if CHECK_REF_COUNTED_LIFECYCLE || ENABLE(SECURITY_ASSERT)
    friend void adopted(RefCountedBase*);
#endif

    int m_refCount;
#if ENABLE(SECURITY_ASSERT)
    bool m_deletionHasBegun;
#endif
#if CHECK_REF_COUNTED_LIFECYCLE
    bool m_adoptionIsRequired;
    ThreadRestrictionVerifier m_verifier;
#endif
};

#if CHECK_REF_COUNTED_LIFECYCLE || ENABLE(SECURITY_ASSERT)
inline void adopted(RefCountedBase* object)
{
    if (!object)
        return;
    ASSERT_WITH_SECURITY_IMPLICATION(!object->m_deletionHasBegun);
#if CHECK_REF_COUNTED_LIFECYCLE
    object->m_adoptionIsRequired = false;
#endif
}
#endif

template<typename T> class RefCounted : public RefCountedBase {
    WTF_MAKE_NONCOPYABLE(RefCounted);

    // Put |T| in here instead of |RefCounted| so the heap profiler reports |T|
    // instead of |RefCounted<T>|. This does not affect overloading of operator
    // new.
    USING_FAST_MALLOC(T);

public:
    void deref()
    {
        if (derefBase())
            delete static_cast<T*>(this);
    }

protected:
#ifdef ENABLE_INSTANCE_COUNTER
    RefCounted()
    {
        incrementInstanceCount<T>(static_cast<T*>(this));
    }

    ~RefCounted()
    {
        decrementInstanceCount<T>(static_cast<T*>(this));
    }
#else
    RefCounted()
    {
    }
#endif
};

} // namespace WTF

using WTF::RefCounted;

#endif // RefCounted_h
