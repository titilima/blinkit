// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: live_node_list_base.cpp
// Description: LiveNodeListBase Class
//      Author: Ziming Li
//     Created: 2019-12-31
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2001 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2004, 2006, 2007, 2008, 2010 Apple Inc. All rights reserved.
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
 */

#include "live_node_list_base.h"

#include "third_party/blink/renderer/core/html/html_collection.h"

namespace blink {

void LiveNodeListBase::InvalidateCacheForAttribute(const QualifiedName *attrName) const
{
    if (IsLiveNodeListType(GetType()))
        ASSERT(false); // BKTODO: ToLiveNodeList(this)->InvalidateCacheForAttribute(attrName);
    else
        ToHTMLCollection(this)->InvalidateCacheForAttribute(attrName);
}

ContainerNode& LiveNodeListBase::RootNode(void) const
{
    if (IsRootedAtTreeScope() && m_ownerNode->IsInTreeScope())
        return m_ownerNode->ContainingTreeScope().RootNode();
    return *m_ownerNode;
}

bool LiveNodeListBase::ShouldInvalidateTypeOnAttributeChange(NodeListInvalidationType type, const QualifiedName &attrName)
{
    switch (type)
    {
        case kInvalidateOnClassAttrChange:
            return attrName == html_names::kClassAttr;
        case kInvalidateOnNameAttrChange:
            return attrName == html_names::kNameAttr;
        case kInvalidateOnIdNameAttrChange:
            return attrName == html_names::kIdAttr || attrName == html_names::kNameAttr;
        case kInvalidateOnForAttrChange:
            return attrName == html_names::kForAttr;
        case kInvalidateForFormControls:
            return attrName == html_names::kNameAttr || attrName == html_names::kIdAttr
                || attrName == html_names::kForAttr || attrName == html_names::kFormAttr
                || attrName == html_names::kTypeAttr;
        case kInvalidateOnHRefAttrChange:
            return attrName == html_names::kHrefAttr;
        case kDoNotInvalidateOnAttributeChanges:
            return false;
        case kInvalidateOnAnyAttrChange:
            return true;
    }
    return false;
}

}  // namespace blink
