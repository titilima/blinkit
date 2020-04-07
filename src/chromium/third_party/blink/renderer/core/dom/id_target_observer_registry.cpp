// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: id_target_observer_registry.cpp
// Description: IdTargetObserverRegistry Class
//      Author: Ziming Li
//     Created: 2020-01-12
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
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

#include "id_target_observer_registry.h"

#include "base/memory/ptr_util.h"
#include "third_party/blink/renderer/core/dom/id_target_observer.h"

namespace blink {

std::unique_ptr<IdTargetObserverRegistry> IdTargetObserverRegistry::Create(void)
{
    return base::WrapUnique(new IdTargetObserverRegistry);
}

void IdTargetObserverRegistry::NotifyObservers(const AtomicString &id)
{
    ASSERT(!m_notifyingObserversInSet);
    if (id.IsEmpty() || m_registry.empty())
        return;
    NotifyObserversInternal(id);
}

void IdTargetObserverRegistry::NotifyObserversInternal(const AtomicString &id)
{
    ASSERT(false); // BKTODO:
}

}  // namespace blink
