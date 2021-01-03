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
#include "third_party/blink/renderer/core/intersection_observer/element_intersection_observer_data.h"
#ifndef BLINKIT_CRAWLER_ONLY
#   include "third_party/blink/renderer/core/animation/element_animations.h"
#   include "third_party/blink/renderer/core/dom/pseudo_element.h"
#   include "third_party/blink/renderer/core/dom/pseudo_element_data.h"
#   if 0 // BKTODO:
#       include "third_party/blink/renderer/core/dom/dataset_dom_string_map.h"
#   else
#       include "third_party/blink/renderer/platform/runtime_enabled_features.h"
#   endif
#   include "third_party/blink/renderer/core/dom/shadow_root.h"
#   include "third_party/blink/renderer/core/resize_observer/resize_observer_data_map.h"
#endif

namespace blink {

class ElementRareData : public NodeRareData
{
public:
#ifdef BLINKIT_CRAWLER_ONLY
    static ElementRareData* Create(void) { return new ElementRareData; }
#else
    static ElementRareData* Create(NodeRenderingData *nodeLayoutData)
    {
        return new ElementRareData(nodeLayoutData);
    }
#endif
    ~ElementRareData(void);
    void TraceAfterDispatch(Visitor *visitor);

    bool HasPseudoElements(void) const
    {
#ifdef BLINKIT_CRAWLER_ONLY
        return false;
#else
        return m_pseudoElementData && m_pseudoElementData->HasPseudoElements();
#endif
    }

    NamedNodeMap* AttributeMap(void) const { return m_attributeMap.Get(); }
    void SetAttributeMap(NamedNodeMap *attributeMap) { m_attributeMap = attributeMap; }

    AttrNodeList& EnsureAttrNodeList(void);
    AttrNodeList* GetAttrNodeList(void) { return m_attrNodeList.get(); }
    void AddAttr(Attr *attr) { EnsureAttrNodeList().push_back(attr); }

    ElementIntersectionObserverData* IntersectionObserverData(void) const { return m_intersectionObserverData.get(); }

    void SetIsValue(const AtomicString &isValue) { m_isValue = isValue; }
    const AtomicString& IsValue(void) const { return m_isValue; }

#ifndef BLINKIT_CRAWLER_ONLY
    PseudoElement* GetPseudoElement(PseudoId pseudoId) const;
    void SetPseudoElement(PseudoId pseudoId, PseudoElement *element);
    void ClearPseudoElements(void);

    ShadowRoot* GetShadowRoot(void) const { return m_shadowRoot.get(); }

    ComputedStyle* GetComputedStyle(void) const { return m_computedStyle.get(); }
    void SetComputedStyle(scoped_refptr<ComputedStyle> computedStyle);
    void ClearComputedStyle(void);

    ElementAnimations* GetElementAnimations(void) { return m_elementAnimations.Get(); }

    ResizeObserverDataMap* ResizeObserverData(void) const { return m_resizeObserverData.get(); }
#endif
private:
#ifdef BLINKIT_CRAWLER_ONLY
    ElementRareData(void);
#else
    explicit ElementRareData(NodeRenderingData*);
#endif

#ifndef BLINKIT_CRAWLER_ONLY
    std::unique_ptr<ShadowRoot> m_shadowRoot;
    scoped_refptr<ComputedStyle> m_computedStyle;
    Member<ElementAnimations> m_elementAnimations;
    std::unique_ptr<ResizeObserverDataMap> m_resizeObserverData;
    Member<PseudoElementData> m_pseudoElementData;
#endif
    Member<NamedNodeMap> m_attributeMap;
    std::unique_ptr<AttrNodeList> m_attrNodeList;

    std::unique_ptr<ElementIntersectionObserverData> m_intersectionObserverData;

    AtomicString m_isValue;
};

} // namespace blink

#endif // BLINKIT_BLINK_ELEMENT_RARE_DATA_H
