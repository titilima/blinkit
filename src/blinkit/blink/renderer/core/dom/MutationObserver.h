#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: MutationObserver.h
// Description: MutationObserver Class
//      Author: Ziming Li
//     Created: 2021-07-21
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2011 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef MutationObserver_h
#define MutationObserver_h

#include "blinkit/blink/renderer/bindings/core/script_wrappable.h"
#include "blinkit/blink/renderer/wtf/HashSet.h"
#include "blinkit/blink/renderer/wtf/PassOwnPtr.h"
#include "blinkit/blink/renderer/wtf/PassRefPtr.h"
#include "blinkit/blink/renderer/wtf/RefCounted.h"
#include "blinkit/blink/renderer/wtf/RefPtr.h"
#include "blinkit/blink/renderer/wtf/Vector.h"

namespace blink {

class ExceptionState;
class MutationCallback;
class MutationObserver;
class MutationObserverInit;
class MutationObserverRegistration;
class MutationRecord;
class Node;

typedef unsigned char MutationObserverOptions;
typedef unsigned char MutationRecordDeliveryOptions;

using MutationObserverSet = WillBeHeapHashSet<RefPtrWillBeMember<MutationObserver>>;
using MutationObserverRegistrationSet = WillBeHeapHashSet<RawPtrWillBeWeakMember<MutationObserverRegistration>>;
using MutationObserverVector = std::vector<Member<MutationObserver>>;
using MutationRecordVector = WillBeHeapVector<RefPtrWillBeMember<MutationRecord>>;

class MutationObserver final : public RefCountedWillBeGarbageCollectedFinalized<MutationObserver>, public ScriptWrappable {
    DEFINE_WRAPPERTYPEINFO();
public:
    enum MutationType {
        ChildList = 1 << 0,
        Attributes = 1 << 1,
        CharacterData = 1 << 2,

        AllMutationTypes = ChildList | Attributes | CharacterData
    };

    enum ObservationFlags  {
        Subtree = 1 << 3,
        AttributeFilter = 1 << 4
    };

    enum DeliveryFlags {
        AttributeOldValue = 1 << 5,
        CharacterDataOldValue = 1 << 6,
    };

    static PassRefPtrWillBeRawPtr<MutationObserver> create(PassOwnPtrWillBeRawPtr<MutationCallback>);
    static void resumeSuspendedObservers();
    static void deliverMutations();

    ~MutationObserver();

    void observe(Node*, const MutationObserverInit&, ExceptionState&);
    MutationRecordVector takeRecords();
    void disconnect();
    void observationStarted(MutationObserverRegistration*);
    void observationEnded(MutationObserverRegistration*);
    void enqueueMutationRecord(PassRefPtrWillBeRawPtr<MutationRecord>);
    void setHasTransientRegistration();

    WillBeHeapHashSet<RawPtrWillBeMember<Node>> getObservedNodes() const;

    // Eagerly finalized as destructor accesses heap object members.
    EAGERLY_FINALIZE();
    DECLARE_TRACE();

private:
    struct ObserverLessThan;

    explicit MutationObserver(PassOwnPtrWillBeRawPtr<MutationCallback>);
    void deliver();
    bool shouldBeSuspended() const;

    OwnPtrWillBeMember<MutationCallback> m_callback;
    MutationRecordVector m_records;
    MutationObserverRegistrationSet m_registrations;
    unsigned m_priority;
};

} // namespace blink

#endif // MutationObserver_h
