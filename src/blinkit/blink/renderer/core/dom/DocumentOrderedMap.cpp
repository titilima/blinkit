// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: DocumentOrderedMap.cpp
// Description: DocumentOrderedMap Class
//      Author: Ziming Li
//     Created: 2021-07-04
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009 Apple Inc. All rights reserved.
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

#include "./DocumentOrderedMap.h"

#include "blinkit/blink/renderer/core/HTMLNames.h"
#include "blinkit/blink/renderer/core/dom/element_traversal.h"
// BKTODO: #include "core/html/HTMLMapElement.h"

namespace blink {

using namespace HTMLNames;

std::unique_ptr<DocumentOrderedMap> DocumentOrderedMap::create()
{
    return zed::wrap_unique(new DocumentOrderedMap);
}

DocumentOrderedMap::DocumentOrderedMap()
{
}

DEFINE_EMPTY_DESTRUCTOR_WILL_BE_REMOVED(DocumentOrderedMap);

#if ENABLE(ASSERT)
static int s_removeScopeLevel = 0;

DocumentOrderedMap::RemoveScope::RemoveScope()
{
    s_removeScopeLevel++;
}

DocumentOrderedMap::RemoveScope::~RemoveScope()
{
    ASSERT(s_removeScopeLevel);
    s_removeScopeLevel--;
}
#endif

inline bool keyMatchesId(const AtomicString& key, const Element& element)
{
    return element.getIdAttribute() == key;
}

inline bool keyMatchesMapName(const AtomicString& key, const Element& element)
{
    ASSERT(false); // BKTODO: return isHTMLMapElement(element) && toHTMLMapElement(element).getName() == key;
    return false;
}

inline bool keyMatchesLowercasedMapName(const AtomicString& key, const Element& element)
{
    ASSERT(false); // BKTODO: return isHTMLMapElement(element) && toHTMLMapElement(element).getName().lower() == key;
    return false;
}

inline bool keyMatchesLabelForAttribute(const AtomicString& key, const Element& element)
{
    ASSERT(false); // BKTODO: return isHTMLLabelElement(element) && element.getAttribute(forAttr) == key;
    return false;
}

void DocumentOrderedMap::add(const AtomicString& key, Element* element)
{
    ASSERT(key);
    ASSERT(element);

    auto it = m_map.find(key);
    if (m_map.end() == it)
    {
        m_map.emplace(key, element);
        return;
    }

    MapEntry &entry = it->second;
    ASSERT(entry.count > 0);
    entry.element = nullptr;
    ++entry.count;
    entry.orderedList.push_back(element);
}

void DocumentOrderedMap::remove(const AtomicString& key, Element* element)
{
    ASSERT(key);
    ASSERT(element);

    Map::iterator it = m_map.find(key);
    if (it == m_map.end())
        return;

    MapEntry &entry = it->second;
    ASSERT(entry.count > 0);
    if (entry.count == 1)
    {
        ASSERT(nullptr == entry.element || entry.element == element);
        m_map.erase(it);
    }
    else
    {
        if (entry.element == element)
        {
            ASSERT(entry.orderedList.empty() || entry.orderedList.front() == element);
            entry.element = entry.orderedList.size() > 1 ? entry.orderedList[1] : nullptr;
        }
        --entry.count;
        entry.orderedList.clear();
    }
}

template<bool keyMatches(const AtomicString&, const Element&)>
inline Element* DocumentOrderedMap::get(const AtomicString& key, const TreeScope* scope) const
{
    ASSERT(key);
    ASSERT(scope);

    MapEntry* entry = zed::find_value(m_map, key);
    if (!entry)
        return 0;

    ASSERT(entry->count);
    if (entry->element)
        return entry->element;

    // Iterate to find the node that matches. Nothing will match iff an element
    // with children having duplicate IDs is being removed -- the tree traversal
    // will be over an updated tree not having that subtree. In all other cases,
    // a match is expected.
    for (Element& element : ElementTraversal::startsAfter(scope->rootNode())) {
        if (!keyMatches(key, element))
            continue;
        entry->element = &element;
        return &element;
    }
    // As get()/getElementById() can legitimately be called while handling element
    // removals, allow failure iff we're in the scope of node removals.
    ASSERT(s_removeScopeLevel);
    return 0;
}

Element* DocumentOrderedMap::getElementById(const AtomicString& key, const TreeScope* scope) const
{
    return get<keyMatchesId>(key, scope);
}

const std::vector<Element *>& DocumentOrderedMap::getAllElementsById(const AtomicString& key, const TreeScope* scope) const
{
    static std::vector<Element *> emptyVector;

    ASSERT(key);
    ASSERT(scope);

    Map::iterator it = m_map.find(key);
    if (it == m_map.end())
        return emptyVector;

    MapEntry &entry = it->second;
    ASSERT(entry.count > 0);

    if (entry.orderedList.empty())
    {
        entry.orderedList.reserve(entry.count);
        for (Element *element = entry.element ? entry.element : ElementTraversal::firstWithin(scope->rootNode());
            entry.orderedList.size() < entry.count;
            element = ElementTraversal::next(*element))
        {
            ASSERT(element);
            if (!keyMatchesId(key, *element))
                continue;
            entry.orderedList.emplace_back(element);
        }
        if (!entry.element)
            entry.element = entry.orderedList.front();
    }

    return entry.orderedList;
}

Element* DocumentOrderedMap::getElementByMapName(const AtomicString& key, const TreeScope* scope) const
{
    return get<keyMatchesMapName>(key, scope);
}

Element* DocumentOrderedMap::getElementByLowercasedMapName(const AtomicString& key, const TreeScope* scope) const
{
    return get<keyMatchesLowercasedMapName>(key, scope);
}

Element* DocumentOrderedMap::getElementByLabelForAttribute(const AtomicString& key, const TreeScope* scope) const
{
    return get<keyMatchesLabelForAttribute>(key, scope);
}

#if 0 // BKTODO:
DEFINE_TRACE(DocumentOrderedMap)
{
#if ENABLE(OILPAN)
    visitor->trace(m_map);
#endif
}

DEFINE_TRACE(DocumentOrderedMap::MapEntry)
{
    visitor->trace(element);
#if ENABLE(OILPAN)
    visitor->trace(orderedList);
#endif
}
#endif

} // namespace blink
