// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: IdTargetObserverRegistry.h
// Description: IdTargetObserverRegistry Class
//      Author: Ziming Li
//     Created: 2021-07-29
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2012 Google Inc. All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef IdTargetObserverRegistry_h
#define IdTargetObserverRegistry_h

#include "platform/heap/Handle.h"
#include "wtf/Forward.h"
#include "wtf/text/StringHash.h"

namespace blink {

class IdTargetObserver;

class IdTargetObserverRegistry final {
    WTF_MAKE_NONCOPYABLE(IdTargetObserverRegistry);
    friend class IdTargetObserver;
public:
    static std::unique_ptr<IdTargetObserverRegistry> create();
    void notifyObservers(const AtomicString& id);
    bool hasObservers(const AtomicString& id) const;
private:
    IdTargetObserverRegistry() : m_notifyingObserversInSet(nullptr) { }
    void addObserver(const AtomicString& id, IdTargetObserver*);
    void removeObserver(const AtomicString& id, IdTargetObserver*);
    void notifyObserversInternal(const AtomicString& id);

    typedef std::unordered_set<IdTargetObserver *> ObserverSet;
    typedef std::unordered_map<StringImpl *, ObserverSet> IdToObserverSetMap;
    IdToObserverSetMap m_registry;
    ObserverSet *m_notifyingObserversInSet = nullptr;
};

inline void IdTargetObserverRegistry::notifyObservers(const AtomicString& id)
{
    ASSERT(nullptr == m_notifyingObserversInSet);
    if (id.isEmpty() || m_registry.empty())
        return;
    IdTargetObserverRegistry::notifyObserversInternal(id);
}

} // namespace blink

#endif // IdTargetObserverRegistry_h
