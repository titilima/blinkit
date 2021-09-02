// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: CustomElementUpgradeCandidateMap.cpp
// Description: CustomElementUpgradeCandidateMap Class
//      Author: Ziming Li
//     Created: 2021-07-20
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2013 Google Inc. All rights reserved.
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

#include "core/dom/custom/CustomElementUpgradeCandidateMap.h"

#include "core/dom/Element.h"

namespace blink {

std::unique_ptr<CustomElementUpgradeCandidateMap> CustomElementUpgradeCandidateMap::create()
{
    return zed::wrap_unique(new CustomElementUpgradeCandidateMap());
}

CustomElementUpgradeCandidateMap::~CustomElementUpgradeCandidateMap()
{
#if !ENABLE(OILPAN)
    // With Oilpan enabled, the observer table keeps a weak reference to the
    // element; no need for explicit removal.
    UpgradeCandidateMap::const_iterator::Keys end = m_upgradeCandidates.end().keys();
    for (UpgradeCandidateMap::const_iterator::Keys it = m_upgradeCandidates.begin().keys(); it != end; ++it)
        unobserve(*it);
#endif
}

void CustomElementUpgradeCandidateMap::add(const CustomElementDescriptor& descriptor, Element* element)
{
    observe(element);

    m_upgradeCandidates.emplace(element, descriptor);

    UnresolvedDefinitionMap::iterator it = m_unresolvedDefinitions.find(descriptor);
    ElementSet* elements;
    if (it == m_unresolvedDefinitions.end())
    {
        elements = adoptPtrWillBeNoop(new ElementSet());
        m_unresolvedDefinitions.emplace(descriptor, elements);
    }
    else
    {
        elements = it->second.get();
    }
    elements->insert(element);
}

void CustomElementUpgradeCandidateMap::elementWasDestroyed(Element* element)
{
    CustomElementObserver::elementWasDestroyed(element);
    UpgradeCandidateMap::iterator candidate = m_upgradeCandidates.find(element);
    ASSERT_WITH_SECURITY_IMPLICATION(candidate != m_upgradeCandidates.end());

    UnresolvedDefinitionMap::iterator elements = m_unresolvedDefinitions.find(candidate->second);
    ASSERT_WITH_SECURITY_IMPLICATION(elements != m_unresolvedDefinitions.end());
    elements->second->erase(element);
    m_upgradeCandidates.erase(candidate);
}

PassOwnPtrWillBeRawPtr<CustomElementUpgradeCandidateMap::ElementSet> CustomElementUpgradeCandidateMap::takeUpgradeCandidatesFor(const CustomElementDescriptor& descriptor)
{
    auto it = m_unresolvedDefinitions.find(descriptor);
    if (m_unresolvedDefinitions.end() != it)
        return nullptr;

    for (const auto& candidate : *(it->second)) {
        unobserve(candidate);
        m_upgradeCandidates.erase(candidate);
    }
    return it->second;
}

DEFINE_TRACE(CustomElementUpgradeCandidateMap)
{
#if ENABLE(OILPAN)
    ASSERT(false); // BKTODO: visitor->trace(m_upgradeCandidates);
    visitor->trace(m_unresolvedDefinitions);
#endif
    CustomElementObserver::trace(visitor);
}

} // namespace blink
