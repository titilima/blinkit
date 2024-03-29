#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: element_rare_data.h
// Description: ElementRareData Class
//      Author: Ziming Li
//     Created: 2021-10-01
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
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

#ifndef ElementRareData_h
#define ElementRareData_h

#include "blinkit/blink/renderer/core/animation/ElementAnimations.h"
#include "blinkit/blink/renderer/core/dom/Attr.h"
#include "blinkit/blink/renderer/core/dom/CompositorProxiedPropertySet.h"
#include "blinkit/blink/renderer/core/dom/DatasetDOMStringMap.h"
#include "blinkit/blink/renderer/core/dom/NamedNodeMap.h"
#include "blinkit/blink/renderer/core/dom/NodeIntersectionObserverData.h"
#include "blinkit/blink/renderer/core/dom/NodeRareData.h"
#include "blinkit/blink/renderer/core/dom/PseudoElement.h"
#include "blinkit/blink/renderer/core/dom/custom/CustomElementDefinition.h"
#include "blinkit/blink/renderer/core/dom/shadow/ElementShadow.h"
#include "blinkit/blink/renderer/core/html/class_list.h"
#include "blinkit/blink/renderer/core/style/StyleInheritedData.h"
#include "blinkit/blink/renderer/platform/heap/Handle.h"
#include "blinkit/blink/renderer/wtf/HashSet.h"
#include "blinkit/blink/renderer/wtf/OwnPtr.h"

namespace blink {

class HTMLElement;
class CompositorProxiedPropertySet;

class ElementRareData : public NodeRareData {
public:
    static ElementRareData* create(LayoutObject* layoutObject)
    {
        return new ElementRareData(layoutObject);
    }

    ~ElementRareData();

    void setPseudoElement(PseudoId, const GCRefPtr<PseudoElement> &);
    PseudoElement* pseudoElement(PseudoId) const;

    short tabIndex() const { return m_tabindex; }

    void setTabIndexExplicitly(short index)
    {
        m_tabindex = index;
        setElementFlag(TabIndexWasSetExplicitly, true);
    }

    void clearTabIndexExplicitly()
    {
        m_tabindex = 0;
        clearElementFlag(TabIndexWasSetExplicitly);
    }

    CSSStyleDeclaration& ensureInlineCSSStyleDeclaration(Element* ownerElement);

    void clearShadow() { m_shadow.reset(); }
    ElementShadow* shadow() const { return m_shadow.get(); }
    ElementShadow& ensureShadow()
    {
        if (!m_shadow)
            m_shadow = ElementShadow::create();
        return *m_shadow;
    }

    NamedNodeMap* attributeMap() const { return m_attributeMap.get(); }
    void setAttributeMap(PassOwnPtrWillBeRawPtr<NamedNodeMap> attributeMap) { m_attributeMap = attributeMap; }

    ComputedStyle* ensureComputedStyle() const { return m_computedStyle.get(); }
    void setComputedStyle(PassRefPtr<ComputedStyle> computedStyle) { m_computedStyle = computedStyle; }
    void clearComputedStyle() { m_computedStyle = nullptr; }

    ClassList* classList() const { return m_classList.get(); }
    void setClassList(std::unique_ptr<ClassList> &&classList) { m_classList = std::move(classList); }
    void clearClassListValueForQuirksMode()
    {
        if (!m_classList)
            return;
        m_classList->clearValueForQuirksMode();
    }

    DatasetDOMStringMap* dataset() const { return m_dataset.get(); }
    void setDataset(PassOwnPtrWillBeRawPtr<DatasetDOMStringMap> dataset) { m_dataset = dataset; }

    LayoutSize minimumSizeForResizing() const { return m_minimumSizeForResizing; }
    void setMinimumSizeForResizing(LayoutSize size) { m_minimumSizeForResizing = size; }

    IntSize savedLayerScrollOffset() const { return m_savedLayerScrollOffset; }
    void setSavedLayerScrollOffset(IntSize size) { m_savedLayerScrollOffset = size; }

#ifdef BLINKIT_UI_ENABLED
    ElementAnimations* elementAnimations() { return m_elementAnimations.get(); }
    void setElementAnimations(std::unique_ptr<ElementAnimations> &&elementAnimations)
    {
        m_elementAnimations = std::move(elementAnimations);
    }
#endif

    bool hasPseudoElements() const;
    void clearPseudoElements();

    void incrementCompositorProxiedProperties(uint32_t properties);
    void decrementCompositorProxiedProperties(uint32_t properties);
    CompositorProxiedPropertySet* proxiedPropertyCounts() const { return m_proxiedProperties.get(); }

    void setCustomElementDefinition(PassRefPtrWillBeRawPtr<CustomElementDefinition> definition) { m_customElementDefinition = definition; }
    CustomElementDefinition* customElementDefinition() const { return m_customElementDefinition.get(); }

    AttrNodeList& ensureAttrNodeList();
    AttrNodeList* attrNodeList() { return m_attrNodeList.get(); }
    void removeAttrNodeList() { m_attrNodeList.reset(); }

    NodeIntersectionObserverData* intersectionObserverData() const { return m_intersectionObserverData.get(); }
    NodeIntersectionObserverData& ensureIntersectionObserverData()
    {
        if (!m_intersectionObserverData)
            m_intersectionObserverData = new NodeIntersectionObserverData();
        return *m_intersectionObserverData;
    }

    DECLARE_TRACE_AFTER_DISPATCH();

private:
    CompositorProxiedPropertySet& ensureCompositorProxiedPropertySet();
    void clearCompositorProxiedPropertySet() { m_proxiedProperties = nullptr; }

    short m_tabindex;

    LayoutSize m_minimumSizeForResizing;
    IntSize m_savedLayerScrollOffset;

    OwnPtrWillBeMember<DatasetDOMStringMap> m_dataset;
    std::unique_ptr<ClassList> m_classList;
    GCUniquePtr<ElementShadow> m_shadow;
    OwnPtrWillBeMember<NamedNodeMap> m_attributeMap;
    std::unique_ptr<AttrNodeList> m_attrNodeList;
    GCRefPtr<InlineCSSStyleDeclaration> m_cssomWrapper;
    OwnPtr<CompositorProxiedPropertySet> m_proxiedProperties;

#ifdef BLINKIT_UI_ENABLED
    std::unique_ptr<ElementAnimations> m_elementAnimations;
#endif
    PersistentWillBeMember<NodeIntersectionObserverData> m_intersectionObserverData;

    RefPtr<ComputedStyle> m_computedStyle;
    RefPtrWillBeMember<CustomElementDefinition> m_customElementDefinition;

    GCRefPtr<PseudoElement> m_generatedBefore;
    GCRefPtr<PseudoElement> m_generatedAfter;
    GCRefPtr<PseudoElement> m_generatedFirstLetter;
    GCRefPtr<PseudoElement> m_backdrop;

    explicit ElementRareData(LayoutObject*);
};

inline LayoutSize defaultMinimumSizeForResizing()
{
    return LayoutSize(LayoutUnit::max(), LayoutUnit::max());
}

inline ElementRareData::ElementRareData(LayoutObject* layoutObject)
    : NodeRareData(layoutObject)
    , m_tabindex(0)
    , m_minimumSizeForResizing(defaultMinimumSizeForResizing())
{
    m_isElementRareData = true;
}

inline ElementRareData::~ElementRareData()
{
#if !ENABLE(OILPAN)
    if (m_elementAnimations)
        m_elementAnimations->dispose();
    if (m_intersectionObserverData)
        m_intersectionObserverData->dispose();
    ASSERT(!m_shadow);
#endif
    ASSERT(!m_generatedBefore);
    ASSERT(!m_generatedAfter);
    ASSERT(!m_generatedFirstLetter);
    ASSERT(!m_backdrop);
}

inline bool ElementRareData::hasPseudoElements() const
{
    return m_generatedBefore || m_generatedAfter || m_backdrop || m_generatedFirstLetter;
}

inline void ElementRareData::clearPseudoElements()
{
    setPseudoElement(BEFORE, nullptr);
    setPseudoElement(AFTER, nullptr);
    setPseudoElement(BACKDROP, nullptr);
    setPseudoElement(FIRST_LETTER, nullptr);
}

inline void ElementRareData::setPseudoElement(PseudoId pseudoId, const GCRefPtr<PseudoElement> &element)
{
    switch (pseudoId) {
    case BEFORE:
        if (m_generatedBefore)
            m_generatedBefore->dispose();
        m_generatedBefore = element;
        break;
    case AFTER:
        if (m_generatedAfter)
            m_generatedAfter->dispose();
        m_generatedAfter = element;
        break;
    case BACKDROP:
        if (m_backdrop)
            m_backdrop->dispose();
        m_backdrop = element;
        break;
    case FIRST_LETTER:
        if (m_generatedFirstLetter)
            m_generatedFirstLetter->dispose();
        m_generatedFirstLetter = element;
        break;
    default:
        ASSERT_NOT_REACHED();
    }
}

inline PseudoElement* ElementRareData::pseudoElement(PseudoId pseudoId) const
{
    switch (pseudoId) {
    case BEFORE:
        return m_generatedBefore.get();
    case AFTER:
        return m_generatedAfter.get();
    case BACKDROP:
        return m_backdrop.get();
    case FIRST_LETTER:
        return m_generatedFirstLetter.get();
    default:
        return 0;
    }
}

inline void ElementRareData::incrementCompositorProxiedProperties(uint32_t properties)
{
    ensureCompositorProxiedPropertySet().increment(properties);
}

inline void ElementRareData::decrementCompositorProxiedProperties(uint32_t properties)
{
    m_proxiedProperties->decrement(properties);
    if (m_proxiedProperties->isEmpty())
        clearCompositorProxiedPropertySet();
}

inline CompositorProxiedPropertySet& ElementRareData::ensureCompositorProxiedPropertySet()
{
    if (!m_proxiedProperties)
        m_proxiedProperties = CompositorProxiedPropertySet::create();
    return *m_proxiedProperties;
}

} // namespace

#endif // ElementRareData_h
