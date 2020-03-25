// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: element_rare_data.h
// Description: ElementRareData Class
//      Author: Ziming Li
//     Created: 2019-11-16
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2008, 2009, 2010 Apple Inc. All rights reserved.
 * Copyright (C) 2008 David Smith <catfish.man@gmail.com>
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

#ifndef BLINKIT_BLINK_ELEMENT_RARE_DATA_H
#define BLINKIT_BLINK_ELEMENT_RARE_DATA_H

#pragma once

#include "third_party/blink/renderer/core/dom/named_node_map.h"
#include "third_party/blink/renderer/core/dom/node_rare_data.h"
#include "third_party/blink/renderer/core/dom/pseudo_element_data.h"
#include "third_party/blink/renderer/core/intersection_observer/element_intersection_observer_data.h"

namespace blink {

class ElementRareData : public NodeRareData
{
public:
#ifdef BLINKIT_CRAWLER_ONLY
    static ElementRareData* Create(void) { return new ElementRareData; }
#else
    static ElementRareData* Create(NodeRenderingData* node_layout_data) {
        return new ElementRareData(node_layout_data);
    }
#endif
    ~ElementRareData(void);

    bool HasPseudoElements(void) const
    {
        ASSERT(!m_pseudoElementData); // BKTODO:
        return false;
    }

    NamedNodeMap* AttributeMap(void) const { return m_attributeMap.get(); }
    void SetAttributeMap(std::unique_ptr<NamedNodeMap> &attributeMap)
    {
        m_attributeMap = std::move(attributeMap);
    }

    AttrNodeList& EnsureAttrNodeList(void);
    AttrNodeList* GetAttrNodeList(void) { return m_attrNodeList.get(); }
    void AddAttr(Attr *attr) { EnsureAttrNodeList().push_back(attr); }

    ElementIntersectionObserverData* IntersectionObserverData(void) const { return m_intersectionObserverData.get(); }
private:
#ifdef BLINKIT_CRAWLER_ONLY
    ElementRareData(void) = default;
#else
    explicit ElementRareData(NodeRenderingData*);
#endif

    std::unique_ptr<NamedNodeMap> m_attributeMap;
    std::unique_ptr<AttrNodeList> m_attrNodeList;

    std::unique_ptr<ElementIntersectionObserverData> m_intersectionObserverData;

    std::unique_ptr<PseudoElementData> m_pseudoElementData;
};

} // namespace blink

#endif // BLINKIT_BLINK_ELEMENT_RARE_DATA_H
