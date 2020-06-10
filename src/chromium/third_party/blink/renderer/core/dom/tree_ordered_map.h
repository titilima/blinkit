// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: tree_ordered_map.h
// Description: TreeOrderedMap Class
//      Author: Ziming Li
//     Created: 2020-01-11
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009, 2010 Apple Inc. All rights
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

#ifndef BLINKIT_BLINK_TREE_ORDERED_MAP_H
#define BLINKIT_BLINK_TREE_ORDERED_MAP_H

#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>
#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/wtf/text/atomic_string.h"

namespace blink {

class Element;
class TreeScope;

class TreeOrderedMap : public GarbageCollected<TreeOrderedMap>
{
public:
    static std::unique_ptr<TreeOrderedMap> Create(void);

    void Add(const AtomicString &key, Element &element);
    Element* GetElementById(const AtomicString &key, const TreeScope &scope) const;
    const std::vector<Member<Element>>& GetAllElementsById(const AtomicString &key, const TreeScope &scope) const;

    bool ContainsMultiple(const AtomicString &id) const;

    static const std::vector<Member<Element>>& EmptyElementVector(void);

    // While removing a ContainerNode, ID lookups won't be precise should the tree
    // have elements with duplicate IDs contained in the element being removed.
    // Rare trees, but ID lookups may legitimately fail across such removals;
    // this scope object informs TreeOrderedMaps about the transitory state of the
    // underlying tree.
    class RemoveScope
    {
        STACK_ALLOCATED();
    public:
        RemoveScope(void)
        {
#if DCHECK_IS_ON()
            ++s_removeScopeLevel;
#endif
        }
        ~RemoveScope(void)
        {
#if DCHECK_IS_ON()
            ASSERT(s_removeScopeLevel > 0);
            --s_removeScopeLevel;
#endif
        }
    private:
#if DCHECK_IS_ON()
        static int s_removeScopeLevel;
#endif
    };
private:
    TreeOrderedMap(void) = default;

    typedef std::function<bool(const AtomicString &, const Element &)> Matcher;
    Element* Get(const AtomicString &key, const TreeScope &scope, const Matcher &matcher) const;

    struct MapEntry {
        explicit MapEntry(Element &firstElement) : element(firstElement) {}

        Member<Element> element;
        unsigned count = 1;
        std::vector<Member<Element>> orderedList;
    };
    mutable std::unordered_map<AtomicString, MapEntry> m_map;
};

} // namespace blink

#endif // BLINKIT_BLINK_TREE_ORDERED_MAP_H
