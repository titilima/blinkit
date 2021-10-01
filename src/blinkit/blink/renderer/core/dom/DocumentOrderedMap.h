// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: DocumentOrderedMap.h
// Description: DocumentOrderedMap Class
//      Author: Ziming Li
//     Created: 2021-07-04
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009, 2010 Apple Inc. All rights reserved.
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

#ifndef DocumentOrderedMap_h
#define DocumentOrderedMap_h

#include "platform/heap/Handle.h"
#include "wtf/Allocator.h"
#include "wtf/Forward.h"
#include "wtf/HashMap.h"
#include "wtf/text/AtomicString.h"
#include "wtf/text/AtomicStringHash.h"
#include "wtf/text/StringImpl.h"

namespace blink {

class Element;
class TreeScope;

class DocumentOrderedMap {
    USING_FAST_MALLOC_WILL_BE_REMOVED(DocumentOrderedMap);
    DECLARE_EMPTY_DESTRUCTOR_WILL_BE_REMOVED(DocumentOrderedMap);
public:
    static std::unique_ptr<DocumentOrderedMap> create();

    void add(const AtomicString&, Element*);
    void remove(const AtomicString&, Element*);

    bool contains(const AtomicString&) const;
    bool containsMultiple(const AtomicString&) const;
    // concrete instantiations of the get<>() method template
    Element* getElementById(const AtomicString&, const TreeScope*) const;
    const std::vector<Element *>& getAllElementsById(const AtomicString&, const TreeScope*) const;
    Element* getElementByMapName(const AtomicString&, const TreeScope*) const;
    Element* getElementByLowercasedMapName(const AtomicString&, const TreeScope*) const;
    Element* getElementByLabelForAttribute(const AtomicString&, const TreeScope*) const;

    // BKTODO: DECLARE_TRACE();

#if ENABLE(ASSERT)
    // While removing a ContainerNode, ID lookups won't be precise should the tree
    // have elements with duplicate IDs contained in the element being removed.
    // Rare trees, but ID lookups may legitimately fail across such removals;
    // this scope object informs DocumentOrderedMaps about the transitory
    // state of the underlying tree.
    class RemoveScope {
        STACK_ALLOCATED();
    public:
        RemoveScope();
        ~RemoveScope();
    };
#else
    class RemoveScope {
        STACK_ALLOCATED();
    public:
        RemoveScope() { }
        ~RemoveScope() { }
    };
#endif

private:
    DocumentOrderedMap();

    template<bool keyMatches(const AtomicString&, const Element&)>
    Element* get(const AtomicString&, const TreeScope*) const;

    struct MapEntry {
    public:
        explicit MapEntry(Element *firstElement) : element(firstElement) {}
        // BKTODO: DECLARE_TRACE();

        Element *element;
        unsigned count = 1;
        std::vector<Element *> orderedList;
    };

    using Map = std::unordered_map<AtomicString, MapEntry>;

    mutable Map m_map;
};

inline bool DocumentOrderedMap::contains(const AtomicString& id) const
{
    return zed::key_exists(m_map, id);
}

inline bool DocumentOrderedMap::containsMultiple(const AtomicString& id) const
{
    if (MapEntry *entry = zed::find_value(m_map, id))
        return entry->count > 1;
    return false;
}

} // namespace blink

#endif // DocumentOrderedMap_h
