// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: StyleSheetInvalidationAnalysis.cpp
// Description: StyleSheetInvalidationAnalysis Class
//      Author: Ziming Li
//     Created: 2021-07-29
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2012 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "./StyleSheetInvalidationAnalysis.h"

#include "blinkit/blink/renderer/core/css/CSSSelectorList.h"
#include "blinkit/blink/renderer/core/css/StyleRuleImport.h"
#include "blinkit/blink/renderer/core/css/StyleSheetContents.h"
#include "blinkit/blink/renderer/core/dom/element_traversal.h"
#include "blinkit/blink/renderer/core/dom/TreeScope.h"
#include "blinkit/blink/renderer/core/dom/shadow/ShadowRoot.h"
#include "blinkit/blink/renderer/core/html/HTMLStyleElement.h"

using namespace BlinKit;

namespace blink {

StyleSheetInvalidationAnalysis::StyleSheetInvalidationAnalysis(const TreeScope& treeScope, const std::vector<GCRefPtr<StyleSheetContents>>& sheets)
    : m_treeScope(&treeScope)
{
    for (unsigned i = 0; i < sheets.size() && !m_dirtiesAllStyle; ++i)
        analyzeStyleSheet(sheets[i].get());
}

static bool determineSelectorScopes(const CSSSelectorList& selectorList, std::unordered_set<StringImpl*>& idScopes, std::unordered_set<StringImpl*>& classScopes)
{
    for (const CSSSelector* selector = selectorList.first(); selector; selector = CSSSelectorList::next(*selector)) {
        const CSSSelector* scopeSelector = 0;
        // This picks the widest scope, not the narrowest, to minimize the number of found scopes.
        for (const CSSSelector* current = selector; current; current = current->tagHistory()) {
            // Prefer ids over classes.
            if (current->match() == CSSSelector::Id)
                scopeSelector = current;
            else if (current->match() == CSSSelector::Class && (!scopeSelector || scopeSelector->match() != CSSSelector::Id))
                scopeSelector = current;
            CSSSelector::Relation relation = current->relation();
            // FIXME: it would be better to use setNeedsStyleRecalc for all shadow hosts matching
            // scopeSelector. Currently requests full style recalc.
            if (relation == CSSSelector::ShadowDeep || relation == CSSSelector::ShadowPseudo)
                return false;
            if (relation != CSSSelector::Descendant && relation != CSSSelector::Child && relation != CSSSelector::SubSelector)
                break;
        }
        if (!scopeSelector)
            return false;
        ASSERT(scopeSelector->match() == CSSSelector::Class || scopeSelector->match() == CSSSelector::Id);
        if (scopeSelector->match() == CSSSelector::Id)
            idScopes.emplace(scopeSelector->value().impl());
        else
            classScopes.emplace(scopeSelector->value().impl());
    }
    return true;
}

static bool ruleAdditionMightRequireDocumentStyleRecalc(StyleRuleBase* rule)
{
    // This funciton is conservative. We only return false when we know that
    // the added @rule can't require style recalcs.
    switch (rule->type()) {
    case StyleRule::Import: // Whatever we import should do its own analysis, we don't need to invalidate the document here!
    case StyleRule::Page: // Page rules apply only during printing, we force a full-recalc before printing.
        return false;

    case StyleRule::Media: // If the media rule doesn't apply, we could avoid recalc.
    case StyleRule::FontFace: // If the fonts aren't in use, we could avoid recalc.
    case StyleRule::Supports: // If we evaluated the supports-clause we could avoid recalc.
    case StyleRule::Viewport: // If the viewport doesn't match, we could avoid recalcing.
    case StyleRule::Keyframes: // If the animation doesn't match an element, we could avoid recalc.
        return true;

    // These should all be impossible to reach:
    case StyleRule::Charset:
    case StyleRule::Keyframe:
    case StyleRule::Namespace:
    case StyleRule::Style:
        break;
    }
    ASSERT_NOT_REACHED();
    return true;
}

void StyleSheetInvalidationAnalysis::analyzeStyleSheet(StyleSheetContents* styleSheetContents)
{
    // Updating the style on the shadow DOM for image fallback content can bring us here when imports
    // are still getting loaded in the main document. Just need to exit early as we will return here
    // when the imports finish loading.
    if (styleSheetContents->isLoading())
        return;

    // See if all rules on the sheet are scoped to some specific ids or classes.
    // Then test if we actually have any of those in the tree at the moment.
    const std::vector<GCRefPtr<StyleRuleImport>> &importRules = styleSheetContents->importRules();
    for (unsigned i = 0; i < importRules.size(); ++i) {
        if (!importRules[i]->styleSheet())
            continue;
        analyzeStyleSheet(importRules[i]->styleSheet());
        if (m_dirtiesAllStyle)
            return;
    }

    if (m_treeScope->rootNode().isShadowRoot())
        return;

    const std::vector<GCRefPtr<StyleRuleBase>> &rules = styleSheetContents->childRules();
    for (unsigned i = 0; i < rules.size(); i++) {
        StyleRuleBase* rule = rules[i].get();
        if (!rule->isStyleRule()) {
            if (ruleAdditionMightRequireDocumentStyleRecalc(rule)) {
                m_dirtiesAllStyle = true;
                return;
            }
            continue;
        }
        StyleRule* styleRule = toStyleRule(rule);
        if (!determineSelectorScopes(styleRule->selectorList(), m_idScopes, m_classScopes)) {
            m_dirtiesAllStyle = true;
            return;
        }
    }
}

static bool elementMatchesSelectorScopes(const Element* element, const std::unordered_set<StringImpl*>& idScopes, const std::unordered_set<StringImpl*>& classScopes)
{
    if (!idScopes.empty() && element->hasID() && zed::key_exists(idScopes, element->idForStyleResolution().impl()))
        return true;
    if (classScopes.empty() || !element->hasClass())
        return false;
    const SpaceSplitString& classNames = element->classNames();
    for (unsigned i = 0; i < classNames.size(); ++i) {
        if (zed::key_exists(classScopes, classNames[i].impl()))
            return true;
    }
    return false;
}

void StyleSheetInvalidationAnalysis::invalidateStyle()
{
    ASSERT(!m_dirtiesAllStyle);

    if (m_treeScope->rootNode().isShadowRoot()) {
        ContainerNode* shadowHost = toShadowRoot(m_treeScope->rootNode()).host();
        shadowHost->setNeedsStyleRecalc(SubtreeStyleChange, StyleChangeReasonForTracing::create(StyleChangeReason::StyleSheetChange));
        return;
    }

    if (m_idScopes.empty() && m_classScopes.empty())
        return;
    Element* element = ElementTraversal::firstWithin(m_treeScope->document());
    while (element) {
        if (elementMatchesSelectorScopes(element, m_idScopes, m_classScopes)) {
            element->setNeedsStyleRecalc(SubtreeStyleChange, StyleChangeReasonForTracing::create(StyleChangeReason::StyleSheetChange));
            // The whole subtree is now invalidated, we can skip to the next sibling.
            element = ElementTraversal::nextSkippingChildren(*element);
            continue;
        }
        element = ElementTraversal::next(*element);
    }
}

}
