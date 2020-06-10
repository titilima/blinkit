// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: tree_ordered_map.cpp
// Description: TreeOrderedMap Class
//      Author: Ziming Li
//     Created: 2020-01-11
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009 Apple Inc. All rights
 * reserved.
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

#include "tree_ordered_map.h"

#include "base/memory/ptr_util.h"
#include "third_party/blink/renderer/core/dom/element_traversal.h"

namespace blink {

inline bool KeyMatchesId(const AtomicString &key, const Element &element)
{
    return element.GetIdAttribute() == key;
}

#if DCHECK_IS_ON()
int TreeOrderedMap::RemoveScope::s_removeScopeLevel = 0;
#endif

void TreeOrderedMap::Add(const AtomicString &key, Element &element)
{
    ASSERT(key);

    auto it = m_map.find(key);
    if (std::end(m_map) == it)
    {
        m_map.insert(std::make_pair(key, MapEntry(element)));
        return;
    }

    MapEntry &entry = it->second;
    ASSERT(0 != entry.count);
    entry.element = nullptr;
    ++entry.count;
    entry.orderedList.clear();
}

std::unique_ptr<TreeOrderedMap> TreeOrderedMap::Create(void)
{
    return base::WrapUnique(new TreeOrderedMap);
}

bool TreeOrderedMap::ContainsMultiple(const AtomicString &id) const
{
    const auto it = m_map.find(id);
    return std::end(m_map) != it && it->second.count > 1;
}

const std::vector<Member<Element>>& TreeOrderedMap::EmptyElementVector(void)
{
    static std::vector<Member<Element>> emptyVector;
    return emptyVector;
}

Element* TreeOrderedMap::Get(const AtomicString &key, const TreeScope &scope, const Matcher &matcher) const
{
    ASSERT(key);

    const auto it = m_map.find(key);
    if (std::end(m_map) == it)
        return nullptr;

    MapEntry &entry = const_cast<MapEntry &>(it->second);
    ASSERT(entry.count > 0);
    if (nullptr != entry.element)
        return entry.element;

    // Iterate to find the node that matches. Nothing will match iff an element
    // with children having duplicate IDs is being removed -- the tree traversal
    // will be over an updated tree not having that subtree. In all other cases,
    // a match is expected.
    for (Element &element : ElementTraversal::StartsAfter(scope.RootNode()))
    {
        if (!matcher(key, element))
            continue;
        entry.element = &element;
        return &element;
    }
    return nullptr;
}

const std::vector<Member<Element>>& TreeOrderedMap::GetAllElementsById(const AtomicString &key, const TreeScope &scope) const
{
    ASSERT(key);

    auto it = m_map.find(key);
    if (std::end(m_map) == it)
        return EmptyElementVector();

    MapEntry &entry = it->second;
    ASSERT(entry.count > 0);

    if (entry.orderedList.empty())
    {
        entry.orderedList.reserve(entry.count);

        for (Element *element = entry.element ? entry.element.Get() : ElementTraversal::FirstWithin(scope.RootNode());
            entry.orderedList.size() < entry.count;
            element = ElementTraversal::Next(*element))
        {
            ASSERT(nullptr != element);
            if (!KeyMatchesId(key, *element))
                continue;
            entry.orderedList.push_back(element);
        }

        if (!entry.element)
            entry.element = entry.orderedList.front();
    }

    return entry.orderedList;
}

Element* TreeOrderedMap::GetElementById(const AtomicString &key, const TreeScope &scope) const
{
    const auto matcher = [](const AtomicString &key, const Element &element)
    {
        return element.GetIdAttribute() == key;
    };
    return Get(key, scope, matcher);
}

} // namespace blink
