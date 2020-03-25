// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: html_collection.cpp
// Description: HTMLCollection Class
//      Author: Ziming Li
//     Created: 2020-03-04
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 * Copyright (C) 2003-2008, 2011, 2012, 2014 Apple Inc. All rights reserved.
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

#include "html_collection.h"

#include "third_party/blink/renderer/core/dom/class_collection.h"
#include "third_party/blink/renderer/core/html/html_tag_collection.h"

namespace blink {

static NodeListInvalidationType InvalidationTypeExcludingIdAndNameAttributes(CollectionType type)
{
    switch (type)
    {
        case kTagCollectionType:
        case kTagCollectionNSType:
        case kHTMLTagCollectionType:
        case kDocImages:
        case kDocEmbeds:
        case kDocForms:
        case kDocScripts:
        case kDocAll:
        case kNodeChildren:
        case kTableTBodies:
        case kTSectionRows:
        case kTableRows:
        case kTRCells:
        case kSelectOptions:
        case kMapAreas:
            return kDoNotInvalidateOnAttributeChanges;
        case kDocApplets:
        case kSelectedOptions:
        case kDataListOptions:
            // FIXME: We can do better some day.
            return kInvalidateOnAnyAttrChange;
        case kDocAnchors:
            return kInvalidateOnNameAttrChange;
        case kDocLinks:
            return kInvalidateOnHRefAttrChange;
        case kWindowNamedItems:
            return kInvalidateOnIdNameAttrChange;
        case kDocumentNamedItems:
            return kInvalidateOnIdNameAttrChange;
        case kDocumentAllNamedItems:
            return kInvalidateOnIdNameAttrChange;
        case kFormControls:
            return kInvalidateForFormControls;
        case kClassCollectionType:
            return kInvalidateOnClassAttrChange;
        case kNameNodeListType:
        case kRadioNodeListType:
        case kRadioImgNodeListType:
        case kLabelsNodeListType:
            break;
    }
    NOTREACHED();
    return kDoNotInvalidateOnAttributeChanges;
}

static NodeListSearchRoot SearchRootFromCollectionType(const ContainerNode &owner, CollectionType type)
{
    switch (type)
    {
        case kDocImages:
        case kDocApplets:
        case kDocEmbeds:
        case kDocForms:
        case kDocLinks:
        case kDocAnchors:
        case kDocScripts:
        case kDocAll:
        case kWindowNamedItems:
        case kDocumentNamedItems:
        case kDocumentAllNamedItems:
        case kClassCollectionType:
        case kTagCollectionType:
        case kTagCollectionNSType:
        case kHTMLTagCollectionType:
        case kNodeChildren:
        case kTableTBodies:
        case kTSectionRows:
        case kTableRows:
        case kTRCells:
        case kSelectOptions:
        case kSelectedOptions:
        case kDataListOptions:
        case kMapAreas:
            return NodeListSearchRoot::kOwnerNode;
        case kFormControls:
            ASSERT(false); // BKTODO:
#if 0
            if (IsHTMLFieldSetElement(owner))
                return NodeListSearchRoot::kOwnerNode;
            ASSERT(IsHTMLFormElement(owner));
#endif
            return NodeListSearchRoot::kTreeScope;
        case kNameNodeListType:
        case kRadioNodeListType:
        case kRadioImgNodeListType:
        case kLabelsNodeListType:
            break;
    }
    NOTREACHED();
    return NodeListSearchRoot::kOwnerNode;
}

static bool ShouldTypeOnlyIncludeDirectChildren(CollectionType type)
{
    switch (type)
    {
        case kClassCollectionType:
        case kTagCollectionType:
        case kTagCollectionNSType:
        case kHTMLTagCollectionType:
        case kDocAll:
        case kDocAnchors:
        case kDocApplets:
        case kDocEmbeds:
        case kDocForms:
        case kDocImages:
        case kDocLinks:
        case kDocScripts:
        case kDocumentNamedItems:
        case kDocumentAllNamedItems:
        case kMapAreas:
        case kTableRows:
        case kSelectOptions:
        case kSelectedOptions:
        case kDataListOptions:
        case kWindowNamedItems:
        case kFormControls:
            return false;
        case kNodeChildren:
        case kTRCells:
        case kTSectionRows:
        case kTableTBodies:
            return true;
        case kNameNodeListType:
        case kRadioNodeListType:
        case kRadioImgNodeListType:
        case kLabelsNodeListType:
            break;
    }
    NOTREACHED();
    return false;
}

HTMLCollection::HTMLCollection(ContainerNode &ownerNode, CollectionType type, ItemAfterOverrideType itemAfterOverrideType)
    : LiveNodeListBase(ownerNode, SearchRootFromCollectionType(ownerNode, type), InvalidationTypeExcludingIdAndNameAttributes(type), type)
    , m_overridesItemAfter(itemAfterOverrideType == kOverridesItemAfter)
    , m_shouldOnlyIncludeDirectChildren(ShouldTypeOnlyIncludeDirectChildren(type))
{
    // Keep this in the child class because |registerNodeList| requires wrapper
    // tracing and potentially calls virtual methods which is not allowed in a
    // base class constructor.
    GetDocument().RegisterNodeList(this);
}

bool HTMLCollection::ElementMatches(const Element &element) const
{
    // These collections apply to any kind of Elements, not just HTMLElements.
    switch (GetType())
    {
        case kDocAll:
        case kNodeChildren:
            return true;
        case kClassCollectionType:
            return ToClassCollection(*this).ElementMatches(element);
        case kTagCollectionType:
            return ToTagCollection(*this).ElementMatches(element);
        case kHTMLTagCollectionType:
            return ToHTMLTagCollection(*this).ElementMatches(element);
        case kTagCollectionNSType:
            return ToTagCollectionNS(*this).ElementMatches(element);
        case kWindowNamedItems:
            ASSERT(false); // BKTODO:
#if 0
            return ToWindowNameCollection(*this).ElementMatches(element);
#endif
        case kDocumentAllNamedItems:
            ASSERT(false); // BKTODO:
#if 0
            return ToDocumentAllNameCollection(*this).ElementMatches(element);
#endif
        default:
            break;
    }

    ASSERT(false); // BKTODO:
    return false;
#if 0
    // The following only applies to HTMLElements.
    return element.IsHTMLElement() &&
        IsMatchingHTMLElement(*this, ToHTMLElement(element));
#endif
}

void HTMLCollection::InvalidateCache(Document *oldDocument) const
{
    m_collectionItemsCache.Invalidate();
    InvalidateIdNameCacheMaps(oldDocument);
}

void HTMLCollection::InvalidateCacheForAttribute(const QualifiedName *attrName) const
{
    if (nullptr == attrName || ShouldInvalidateTypeOnAttributeChange(InvalidationType(), *attrName))
        InvalidateCache();
    else if (*attrName == html_names::kIdAttr || *attrName == html_names::kNameAttr)
        InvalidateIdNameCacheMaps();
}

void HTMLCollection::InvalidateIdNameCacheMaps(Document *oldDocument) const
{
    if (!HasValidIdNameCache())
        return;

    // Make sure we decrement the NodeListWithIdNameCache count from
    // the old document instead of the new one in the case the collection
    // is moved to a new document.
    UnregisterIdNameCacheFromDocument(nullptr != oldDocument ? *oldDocument : GetDocument());

    m_namedItemCache.reset();
}

Element* HTMLCollection::item(unsigned offset) const
{
    Element *element = m_collectionItemsCache.NodeAt(*this, offset);
    if (nullptr != element && element->GetDocument().InDOMNodeRemovedHandler())
    {
        ASSERT(false); // BKTODO:
#if 0
        if (NodeChildRemovalTracker::IsBeingRemoved(element))
            GetDocument().CountDetachingNodeAccessInDOMNodeRemovedHandler();
#endif
    }
    return element;
}

unsigned HTMLCollection::length(void) const
{
    return m_collectionItemsCache.NodeCount(*this);
}

namespace {

template <class HTMLCollectionType>
class IsMatch
{
    STACK_ALLOCATED();
public:
    IsMatch(const HTMLCollectionType &list) : m_list(&list) {}
    bool operator()(const Element &element) const
    {
        return m_list->ElementMatches(element);
    }
private:
    Member<const HTMLCollectionType> m_list;
};

}  // namespace

template <class HTMLCollectionType>
static inline IsMatch<HTMLCollectionType> MakeIsMatch(const HTMLCollectionType &list)
{
    return IsMatch<HTMLCollectionType>(list);
}

Element* HTMLCollection::TraverseBackwardToOffset(unsigned offset, Element &currentElement, unsigned &currentOffset) const
{
    ASSERT(false); // BKTODO:
    return nullptr;
}

Element* HTMLCollection::TraverseForwardToOffset(unsigned offset, Element &currentElement, unsigned &currentOffset) const
{
    ASSERT(currentOffset < offset);
    switch (GetType())
    {
        case kHTMLTagCollectionType:
            return TraverseMatchingElementsForwardToOffset(currentElement, &RootNode(), offset, currentOffset,
                MakeIsMatch(ToHTMLTagCollection(*this)));
        case kClassCollectionType:
            return TraverseMatchingElementsForwardToOffset(currentElement, &RootNode(), offset, currentOffset,
                MakeIsMatch(ToClassCollection(*this)));
        default:
            if (OverridesItemAfter())
            {
                for (Element *next = VirtualItemAfter(&currentElement); nullptr != next; next = VirtualItemAfter(next))
                {
                    if (++currentOffset == offset)
                        return next;
                }
                return nullptr;
            }
            if (ShouldOnlyIncludeDirectChildren())
            {
                IsMatch<HTMLCollection> isMatch(*this);
                for (Element *next = ElementTraversal::NextSibling(currentElement, isMatch); nullptr != next; next = ElementTraversal::NextSibling(*next, isMatch))
                {
                    if (++currentOffset == offset)
                        return next;
                }
                return nullptr;
            }
            return TraverseMatchingElementsForwardToOffset(currentElement, &RootNode(), offset, currentOffset,
                MakeIsMatch(*this));
    }
}

Element* HTMLCollection::TraverseToFirst(void) const
{
    switch (GetType())
    {
        case kHTMLTagCollectionType:
            return ElementTraversal::FirstWithin(RootNode(), MakeIsMatch(ToHTMLTagCollection(*this)));
        case kClassCollectionType:
            return ElementTraversal::FirstWithin(RootNode(), MakeIsMatch(ToClassCollection(*this)));
    }
    if (OverridesItemAfter())
        return VirtualItemAfter(nullptr);
    if (ShouldOnlyIncludeDirectChildren())
        return ElementTraversal::FirstChild(RootNode(), MakeIsMatch(*this));
    return ElementTraversal::FirstWithin(RootNode(), MakeIsMatch(*this));
}

Element* HTMLCollection::TraverseToLast(void) const
{
    ASSERT(false); // BKTODO:
    return nullptr;
}

void HTMLCollection::UnregisterIdNameCacheFromDocument(Document &document) const
{
    ASSERT(false); // BKTODO:
}

Element* HTMLCollection::VirtualItemAfter(Element *element) const
{
    NOTREACHED();
    return nullptr;
}

} // namespace blink
