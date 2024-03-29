// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: StyleInvalidator.cpp
// Description: StyleInvalidator Class
//      Author: Ziming Li
//     Created: 2021-07-19
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "./StyleInvalidator.h"

#include "blinkit/blink/renderer/core/css/invalidation/InvalidationSet.h"
#include "blinkit/blink/renderer/core/dom/element_traversal.h"
#include "blinkit/blink/renderer/core/dom/shadow/ElementShadow.h"
#include "blinkit/blink/renderer/core/dom/shadow/ShadowRoot.h"
#include "blinkit/blink/renderer/core/inspector/InspectorTraceEvents.h"
#include "blinkit/blink/renderer/core/layout/LayoutObject.h"

namespace blink {

#define TRACE_STYLE_INVALIDATOR_INVALIDATION_IF_ENABLED(...)    ((void)0)

void StyleInvalidator::invalidate(Document& document)
{
    RecursionData recursionData;
    SiblingData siblingData;
    if (Element* documentElement = document.documentElement())
        invalidate(*documentElement, recursionData, siblingData);
    document.clearChildNeedsStyleInvalidation();
    document.clearNeedsStyleInvalidation();
    m_pendingInvalidationMap.clear();
}

void StyleInvalidator::scheduleInvalidationSetsForElement(const InvalidationLists& invalidationLists, Element& element)
{
    ASSERT(element.inActiveDocument());
    bool requiresDescendantInvalidation = false;

    if (element.styleChangeType() < SubtreeStyleChange) {
        for (auto& invalidationSet : invalidationLists.descendants) {
            if (invalidationSet->wholeSubtreeInvalid()) {
                element.setNeedsStyleRecalc(SubtreeStyleChange, StyleChangeReasonForTracing::create(StyleChangeReason::StyleInvalidator));
                requiresDescendantInvalidation = false;
                break;
            }

            if (invalidationSet->invalidatesSelf())
                element.setNeedsStyleRecalc(LocalStyleChange, StyleChangeReasonForTracing::create(StyleChangeReason::StyleInvalidator));

            if (!invalidationSet->isEmpty())
                requiresDescendantInvalidation = true;
        }
    }

    if (!requiresDescendantInvalidation && (invalidationLists.siblings.isEmpty() || !element.nextSibling()))
        return;

    element.setNeedsStyleInvalidation();

    PendingInvalidations& pendingInvalidations = ensurePendingInvalidations(element);
    if (element.nextSibling()) {
        for (auto& invalidationSet : invalidationLists.siblings) {
            if (pendingInvalidations.siblings().contains(invalidationSet))
                continue;
            pendingInvalidations.siblings().append(invalidationSet);
        }
    }

    if (!requiresDescendantInvalidation)
        return;

    for (auto& invalidationSet : invalidationLists.descendants) {
        ASSERT(!invalidationSet->wholeSubtreeInvalid());
        if (invalidationSet->isEmpty())
            continue;
        if (pendingInvalidations.descendants().contains(invalidationSet))
            continue;
        pendingInvalidations.descendants().append(invalidationSet);
    }
}

void StyleInvalidator::clearInvalidation(Element& element)
{
    if (!element.needsStyleInvalidation())
        return;
    m_pendingInvalidationMap.erase(&element);
    element.clearNeedsStyleInvalidation();
}

PendingInvalidations& StyleInvalidator::ensurePendingInvalidations(Element& element)
{
    std::unique_ptr<PendingInvalidations> &pendingInvalidations = m_pendingInvalidationMap[&element];
    if (!pendingInvalidations)
        pendingInvalidations = std::make_unique<PendingInvalidations>();
    return *pendingInvalidations;
}

StyleInvalidator::StyleInvalidator()
{
}

StyleInvalidator::~StyleInvalidator()
{
}

void StyleInvalidator::RecursionData::pushInvalidationSet(const DescendantInvalidationSet& invalidationSet)
{
    ASSERT(!m_wholeSubtreeInvalid);
    ASSERT(!invalidationSet.wholeSubtreeInvalid());
    ASSERT(!invalidationSet.isEmpty());
    if (invalidationSet.treeBoundaryCrossing())
        m_treeBoundaryCrossing = true;
    if (invalidationSet.insertionPointCrossing())
        m_insertionPointCrossing = true;
    m_invalidationSets.append(&invalidationSet);
    m_invalidateCustomPseudo = invalidationSet.customPseudoInvalid();
}

ALWAYS_INLINE bool StyleInvalidator::RecursionData::matchesCurrentInvalidationSets(Element& element) const
{
    if (m_invalidateCustomPseudo && element.shadowPseudoId() != nullAtom) {
        TRACE_STYLE_INVALIDATOR_INVALIDATION_IF_ENABLED(element, InvalidateCustomPseudo);
        return true;
    }

    if (m_insertionPointCrossing && element.isInsertionPoint())
        return true;

    for (const auto& invalidationSet : m_invalidationSets) {
        if (invalidationSet->invalidatesElement(element))
            return true;
    }

    return false;
}

void StyleInvalidator::SiblingData::pushInvalidationSet(const SiblingInvalidationSet& invalidationSet)
{
    unsigned invalidationLimit;
    if (invalidationSet.maxDirectAdjacentSelectors() == UINT_MAX)
        invalidationLimit = UINT_MAX;
    else
        invalidationLimit = m_elementIndex + invalidationSet.maxDirectAdjacentSelectors();
    m_invalidationEntries.append(Entry(&invalidationSet, invalidationLimit));
}

bool StyleInvalidator::SiblingData::matchCurrentInvalidationSets(Element& element, RecursionData& recursionData) const
{
    bool thisElementNeedsStyleRecalc = false;
    ASSERT(!recursionData.wholeSubtreeInvalid());

    unsigned index = 0;
    while (index < m_invalidationEntries.size()) {
        if (m_elementIndex > m_invalidationEntries[index].m_invalidationLimit) {
            // m_invalidationEntries[index] only applies to earlier siblings. Remove it.
            m_invalidationEntries[index] = m_invalidationEntries.last();
            m_invalidationEntries.removeLast();
            continue;
        }

        const SiblingInvalidationSet& invalidationSet = *m_invalidationEntries[index].m_invalidationSet;

        if (invalidationSet.invalidatesElement(element)) {
            const DescendantInvalidationSet& descendants = invalidationSet.descendants();
            if (descendants.wholeSubtreeInvalid()) {
                // Avoid directly setting SubtreeStyleChange on element, or ContainerNode::checkForChildrenAdjacentRuleChanges()
                // may propagate the SubtreeStyleChange to our own siblings' subtrees.

                for (Element* child = ElementTraversal::firstChild(element); child; child = ElementTraversal::nextSibling(*child)) {
                    child->setNeedsStyleRecalc(SubtreeStyleChange, StyleChangeReasonForTracing::create(StyleChangeReason::SiblingSelector));
                }
                return true;
            }

            if (descendants.invalidatesSelf())
                thisElementNeedsStyleRecalc = true;

            if (!descendants.isEmpty())
                recursionData.pushInvalidationSet(descendants);
        }

        ++index;
    }
    return thisElementNeedsStyleRecalc;
}

void StyleInvalidator::pushInvalidationSetsForElement(Element& element, RecursionData& recursionData, SiblingData& siblingData)
{
    auto it = m_pendingInvalidationMap.find(&element);
    ASSERT(m_pendingInvalidationMap.end() != it);

    PendingInvalidations* pendingInvalidations = it->second.get();

    for (const auto& invalidationSet : pendingInvalidations->siblings())
        siblingData.pushInvalidationSet(toSiblingInvalidationSet(*invalidationSet));

    if (!pendingInvalidations->descendants().isEmpty()) {
        for (const auto& invalidationSet : pendingInvalidations->descendants())
            recursionData.pushInvalidationSet(toDescendantInvalidationSet(*invalidationSet));
    }
}

ALWAYS_INLINE bool StyleInvalidator::checkInvalidationSetsAgainstElement(Element& element, RecursionData& recursionData, SiblingData& siblingData)
{
    if (element.styleChangeType() >= SubtreeStyleChange || recursionData.wholeSubtreeInvalid()) {
        recursionData.setWholeSubtreeInvalid();
        return false;
    }

    bool thisElementNeedsStyleRecalc = recursionData.matchesCurrentInvalidationSets(element);
    if (UNLIKELY(!siblingData.isEmpty()))
        thisElementNeedsStyleRecalc |= siblingData.matchCurrentInvalidationSets(element, recursionData);

    if (UNLIKELY(element.needsStyleInvalidation()))
        pushInvalidationSetsForElement(element, recursionData, siblingData);
    return thisElementNeedsStyleRecalc;
}

bool StyleInvalidator::invalidateShadowRootChildren(Element& element, RecursionData& recursionData)
{
    bool someChildrenNeedStyleRecalc = false;
    for (ShadowRoot* root = element.youngestShadowRoot(); root; root = root->olderShadowRoot()) {
        if (!recursionData.treeBoundaryCrossing() && !root->childNeedsStyleInvalidation() && !root->needsStyleInvalidation())
            continue;
        SiblingData siblingData;
        for (Element* child = ElementTraversal::firstChild(*root); child; child = ElementTraversal::nextSibling(*child)) {
            bool childRecalced = invalidate(*child, recursionData, siblingData);
            someChildrenNeedStyleRecalc = someChildrenNeedStyleRecalc || childRecalced;
        }
        root->clearChildNeedsStyleInvalidation();
        root->clearNeedsStyleInvalidation();
    }
    return someChildrenNeedStyleRecalc;
}

bool StyleInvalidator::invalidateChildren(Element& element, RecursionData& recursionData)
{
    SiblingData siblingData;
    bool someChildrenNeedStyleRecalc = false;
    if (UNLIKELY(!!element.youngestShadowRoot())) {
        someChildrenNeedStyleRecalc = invalidateShadowRootChildren(element, recursionData);
    }

    for (Element* child = ElementTraversal::firstChild(element); child; child = ElementTraversal::nextSibling(*child)) {
        bool childRecalced = invalidate(*child, recursionData, siblingData);
        someChildrenNeedStyleRecalc = someChildrenNeedStyleRecalc || childRecalced;
    }
    return someChildrenNeedStyleRecalc;
}

bool StyleInvalidator::invalidate(Element& element, RecursionData& recursionData, SiblingData& siblingData)
{
    siblingData.advance();
    RecursionCheckpoint checkpoint(&recursionData);

    bool thisElementNeedsStyleRecalc = checkInvalidationSetsAgainstElement(element, recursionData, siblingData);

    bool someChildrenNeedStyleRecalc = false;
    if (recursionData.hasInvalidationSets() || element.childNeedsStyleInvalidation())
        someChildrenNeedStyleRecalc = invalidateChildren(element, recursionData);

    if (thisElementNeedsStyleRecalc) {
        ASSERT(!recursionData.wholeSubtreeInvalid());
        element.setNeedsStyleRecalc(LocalStyleChange, StyleChangeReasonForTracing::create(StyleChangeReason::StyleInvalidator));
    } else if (recursionData.hasInvalidationSets() && someChildrenNeedStyleRecalc) {
        // Clone the ComputedStyle in order to preserve correct style sharing, if possible. Otherwise recalc style.
        if (LayoutObject* layoutObject = element.layoutObject()) {
            layoutObject->setStyleInternal(ComputedStyle::clone(layoutObject->styleRef()));
        } else {
            TRACE_STYLE_INVALIDATOR_INVALIDATION_IF_ENABLED(element, PreventStyleSharingForParent);
            element.setNeedsStyleRecalc(LocalStyleChange, StyleChangeReasonForTracing::create(StyleChangeReason::StyleInvalidator));
        }
    }

    if (recursionData.insertionPointCrossing() && element.isInsertionPoint())
        element.setNeedsStyleRecalc(SubtreeStyleChange, StyleChangeReasonForTracing::create(StyleChangeReason::StyleInvalidator));

    element.clearChildNeedsStyleInvalidation();
    element.clearNeedsStyleInvalidation();

    return thisElementNeedsStyleRecalc;
}

DEFINE_TRACE(StyleInvalidator)
{
#if ENABLE(OILPAN)
    // BKTODO: visitor->trace(m_pendingInvalidationMap);
#endif
}

} // namespace blink
