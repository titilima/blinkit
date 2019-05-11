// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: ElementRareData.h
// Description: ElementRareData Class
//      Author: Ziming Li
//     Created: 2019-02-05
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

#ifndef ElementRareData_h
#define ElementRareData_h

#include "core/animation/ElementAnimations.h"
#include "core/dom/Attr.h"
#include "core/dom/DatasetDOMStringMap.h"
#include "core/dom/NamedNodeMap.h"
#include "core/dom/NodeIntersectionObserverData.h"
#include "core/dom/NodeRareData.h"
#include "core/dom/PseudoElement.h"
#include "core/dom/shadow/ElementShadow.h"
#include "core/html/ClassList.h"
#include "core/style/StyleInheritedData.h"
#include "platform/heap/Handle.h"
#include "wtf/HashSet.h"
#include "wtf/OwnPtr.h"

namespace blink {

class HTMLElement;
class CompositorProxiedPropertySet;

class ElementRareData : public NodeRareData {
public:
#ifdef BLINKIT_CRAWLER_ONLY
    static ElementRareData* create(void)
    {
        return new ElementRareData;
    }
#else
    static ElementRareData* create(LayoutObject* layoutObject)
    {
        return new ElementRareData(layoutObject);
    }
#endif

    ~ElementRareData();

    void setPseudoElement(PseudoId, PassRefPtrWillBeRawPtr<PseudoElement>);
    PseudoElement* pseudoElement(PseudoId) const;

#ifndef BLINKIT_CRAWLER_ONLY
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

    void clearShadow() { m_shadow = nullptr; }
    ElementShadow* shadow() const { return m_shadow.get(); }
    ElementShadow& ensureShadow()
    {
        if (!m_shadow)
            m_shadow = ElementShadow::create();
        return *m_shadow;
    }
#endif

    NamedNodeMap* attributeMap() const { return m_attributeMap.get(); }
    void setAttributeMap(PassOwnPtrWillBeRawPtr<NamedNodeMap> attributeMap) { m_attributeMap = attributeMap; }

#ifndef BLINKIT_CRAWLER_ONLY
    ComputedStyle* ensureComputedStyle() const { return m_computedStyle.get(); }
    void setComputedStyle(PassRefPtr<ComputedStyle> computedStyle) { m_computedStyle = computedStyle; }
    void clearComputedStyle() { m_computedStyle = nullptr; }
#endif

    ClassList* classList() const { return m_classList.get(); }
    void setClassList(PassOwnPtrWillBeRawPtr<ClassList> classList) { m_classList = classList; }
    void clearClassListValueForQuirksMode()
    {
        if (!m_classList)
            return;
        m_classList->clearValueForQuirksMode();
    }

    DatasetDOMStringMap* dataset() const { return m_dataset.get(); }
    void setDataset(PassOwnPtrWillBeRawPtr<DatasetDOMStringMap> dataset) { m_dataset = dataset; }

#ifndef BLINKIT_CRAWLER_ONLY
    LayoutSize minimumSizeForResizing() const { return m_minimumSizeForResizing; }
    void setMinimumSizeForResizing(LayoutSize size) { m_minimumSizeForResizing = size; }

    IntSize savedLayerScrollOffset() const { return m_savedLayerScrollOffset; }
    void setSavedLayerScrollOffset(IntSize size) { m_savedLayerScrollOffset = size; }

    ElementAnimations* elementAnimations() { return m_elementAnimations.get(); }
    void setElementAnimations(ElementAnimations* elementAnimations)
    {
        m_elementAnimations = elementAnimations;
    }
#endif

    bool hasPseudoElements() const;
    void clearPseudoElements();

    void incrementCompositorProxiedProperties(uint32_t properties);
    void decrementCompositorProxiedProperties(uint32_t properties);
    CompositorProxiedPropertySet* proxiedPropertyCounts(void) const { return nullptr; }

    AttrNodeList& ensureAttrNodeList();
    AttrNodeList* attrNodeList() { return m_attrNodeList.get(); }
    void removeAttrNodeList() { m_attrNodeList.clear(); }

    NodeIntersectionObserverData* intersectionObserverData() const { return m_intersectionObserverData.get(); }
    NodeIntersectionObserverData& ensureIntersectionObserverData()
    {
        if (!m_intersectionObserverData)
            m_intersectionObserverData = new NodeIntersectionObserverData();
        return *m_intersectionObserverData;
    }

    DECLARE_TRACE_AFTER_DISPATCH();

private:
#ifndef BLINKIT_CRAWLER_ONLY
    short m_tabindex;

    LayoutSize m_minimumSizeForResizing;
    IntSize m_savedLayerScrollOffset;
#endif

    OwnPtrWillBeMember<DatasetDOMStringMap> m_dataset;
    OwnPtrWillBeMember<ClassList> m_classList;
#ifndef BLINKIT_CRAWLER_ONLY
    OwnPtrWillBeMember<ElementShadow> m_shadow;
#endif
    OwnPtrWillBeMember<NamedNodeMap> m_attributeMap;
    OwnPtrWillBeMember<AttrNodeList> m_attrNodeList;
#ifndef BLINKIT_CRAWLER_ONLY
    OwnPtrWillBeMember<InlineCSSStyleDeclaration> m_cssomWrapper;

    PersistentWillBeMember<ElementAnimations> m_elementAnimations;
#endif
    PersistentWillBeMember<NodeIntersectionObserverData> m_intersectionObserverData;

#ifndef BLINKIT_CRAWLER_ONLY
    RefPtr<ComputedStyle> m_computedStyle;
#endif

    RefPtrWillBeMember<PseudoElement> m_generatedBefore;
    RefPtrWillBeMember<PseudoElement> m_generatedAfter;
    RefPtrWillBeMember<PseudoElement> m_generatedFirstLetter;
    RefPtrWillBeMember<PseudoElement> m_backdrop;

#ifdef BLINKIT_CRAWLER_ONLY
    explicit ElementRareData(void);
#else
    explicit ElementRareData(LayoutObject*);
#endif
};

inline LayoutSize defaultMinimumSizeForResizing()
{
    return LayoutSize(LayoutUnit::max(), LayoutUnit::max());
}

#ifdef BLINKIT_CRAWLER_ONLY
inline ElementRareData::ElementRareData(void)
#else
inline ElementRareData::ElementRareData(LayoutObject* layoutObject)
    : NodeRareData(layoutObject)
    , m_tabindex(0)
    , m_minimumSizeForResizing(defaultMinimumSizeForResizing())
#endif
{
    m_isElementRareData = true;
}

inline ElementRareData::~ElementRareData()
{
#if !ENABLE(OILPAN)
#ifndef BLINKIT_CRAWLER_ONLY
    if (m_elementAnimations)
        m_elementAnimations->dispose();
#endif
    if (m_intersectionObserverData)
        m_intersectionObserverData->dispose();
#ifndef BLINKIT_CRAWLER_ONLY
    ASSERT(!m_shadow);
#endif
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

inline void ElementRareData::setPseudoElement(PseudoId pseudoId, PassRefPtrWillBeRawPtr<PseudoElement> element)
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
    assert(false); // Not reached!
}

inline void ElementRareData::decrementCompositorProxiedProperties(uint32_t properties)
{
    assert(false); // Not reached!
}

} // namespace

#endif // ElementRareData_h
