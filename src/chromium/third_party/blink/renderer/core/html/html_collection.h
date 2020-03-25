// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: html_collection.h
// Description: HTMLCollection Class
//      Author: Ziming Li
//     Created: 2020-03-04
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 * Copyright (C) 2003, 2004, 2005, 2006, 2007, 2008, 2011, 2012 Apple Inc. All
 * rights reserved.
 * Copyright (C) 2014 Samsung Electronics. All rights reserved.
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

#ifndef BLINKIT_BLINK_HTML_COLLECTION_H
#define BLINKIT_BLINK_HTML_COLLECTION_H

#pragma once

#include "third_party/blink/renderer/core/dom/live_node_list_base.h"
#include "third_party/blink/renderer/core/html/collection_items_cache.h"
#include "third_party/blink/renderer/core/html/collection_type.h"

namespace blink {

// blink::HTMLCollection implements HTMLCollection IDL interface.
class HTMLCollection : public ScriptWrappable, public LiveNodeListBase
{
public:
    enum ItemAfterOverrideType {
        kOverridesItemAfter,
        kDoesNotOverrideItemAfter,
    };

    void InvalidateCache(Document *oldDocument = nullptr) const override;
    void InvalidateCacheForAttribute(const QualifiedName *attrName) const;

    // DOM API
    unsigned length(void) const;
    Element* item(unsigned offset) const;

    // Non-DOM API
    bool IsEmpty(void) const { return m_collectionItemsCache.IsEmpty(*this); }
    bool HasExactlyOneItem(void) const
    {
        return m_collectionItemsCache.HasExactlyOneNode(*this);
    }
    bool ElementMatches(const Element &element) const;

    // CollectionIndexCache API.
    bool CanTraverseBackward(void) const { return !OverridesItemAfter(); }
    Element* TraverseToFirst(void) const;
    Element* TraverseToLast(void) const;
    Element* TraverseForwardToOffset(unsigned offset, Element &currentElement, unsigned &currentOffset) const;
    Element* TraverseBackwardToOffset(unsigned offset, Element &currentElement, unsigned &currentOffset) const;
protected:
    HTMLCollection(ContainerNode &ownerNode, CollectionType type, ItemAfterOverrideType itemAfterOverrideType);

    class NamedItemCache final : public GarbageCollected<NamedItemCache>
    {
    };
private:
    bool HasValidIdNameCache(void) const { return !!m_namedItemCache; }
    void InvalidateIdNameCacheMaps(Document *oldDocument = nullptr) const;
    void UnregisterIdNameCacheFromDocument(Document &document) const;

    bool OverridesItemAfter(void) const { return m_overridesItemAfter; }
    virtual Element* VirtualItemAfter(Element *element) const;
    bool ShouldOnlyIncludeDirectChildren(void) const { return m_shouldOnlyIncludeDirectChildren; }

    const unsigned m_overridesItemAfter : 1;
    const unsigned m_shouldOnlyIncludeDirectChildren : 1;
    mutable std::unique_ptr<NamedItemCache> m_namedItemCache;
    mutable CollectionItemsCache<HTMLCollection, Element> m_collectionItemsCache;
};

DEFINE_TYPE_CASTS(HTMLCollection, LiveNodeListBase, collection, IsHTMLCollectionType(collection->GetType()), IsHTMLCollectionType(collection.GetType()));

} // namespace blink

#endif // BLINKIT_BLINK_HTML_COLLECTION_H
