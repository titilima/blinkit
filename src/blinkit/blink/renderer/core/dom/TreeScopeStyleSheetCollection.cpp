// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: TreeScopeStyleSheetCollection.cpp
// Description: TreeScopeStyleSheetCollection Class
//      Author: Ziming Li
//     Created: 2021-08-21
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2001 Dirk Mueller (mueller@kde.org)
 *           (C) 2006 Alexey Proskuryakov (ap@webkit.org)
 * Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2012 Apple Inc. All rights reserved.
 * Copyright (C) 2008, 2009 Torch Mobile Inc. All rights reserved. (http://www.torchmobile.com/)
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies)
 * Copyright (C) 2013 Google Inc. All rights reserved.
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

#include "core/dom/TreeScopeStyleSheetCollection.h"

#include "core/css/CSSStyleSheet.h"
#include "core/css/StyleRuleImport.h"
#include "core/css/StyleSheetContents.h"
#include "core/css/invalidation/StyleSheetInvalidationAnalysis.h"
#include "core/css/resolver/StyleResolver.h"
#include "core/dom/Element.h"
#include "core/dom/StyleEngine.h"
#include "core/html/HTMLLinkElement.h"
#include "core/html/HTMLStyleElement.h"

using namespace BlinKit;

namespace blink {

TreeScopeStyleSheetCollection::TreeScopeStyleSheetCollection(TreeScope& treeScope)
    : m_treeScope(treeScope)
    , m_hadActiveLoadingStylesheet(false)
{
}

void TreeScopeStyleSheetCollection::addStyleSheetCandidateNode(Node* node, bool createdByParser)
{
    if (!node->inDocument())
        return;

    m_styleSheetCandidateNodes.add(node);
}

TreeScopeStyleSheetCollection::StyleResolverUpdateType TreeScopeStyleSheetCollection::compareStyleSheets(const std::vector<GCRefPtr<CSSStyleSheet>>& oldStyleSheets, const std::vector<GCRefPtr<CSSStyleSheet>>& newStylesheets, std::vector<GCRefPtr<StyleSheetContents>>& addedSheets)
{
    unsigned newStyleSheetCount = newStylesheets.size();
    unsigned oldStyleSheetCount = oldStyleSheets.size();
    ASSERT(newStyleSheetCount >= oldStyleSheetCount);

    if (!newStyleSheetCount)
        return Reconstruct;

    unsigned newIndex = 0;
    for (unsigned oldIndex = 0; oldIndex < oldStyleSheetCount; ++oldIndex) {
        while (oldStyleSheets[oldIndex] != newStylesheets[newIndex]) {
            addedSheets.emplace_back(newStylesheets[newIndex]->contents());
            if (++newIndex == newStyleSheetCount)
                return Reconstruct;
        }
        if (++newIndex == newStyleSheetCount)
            return Reconstruct;
    }
    bool hasInsertions = !addedSheets.empty();
    while (newIndex < newStyleSheetCount) {
        addedSheets.emplace_back(newStylesheets[newIndex]->contents());
        ++newIndex;
    }
    // If all new sheets were added at the end of the list we can just add them to existing StyleResolver.
    // If there were insertions we need to re-add all the stylesheets so rules are ordered correctly.
    return hasInsertions ? Reset : Additive;
}

bool TreeScopeStyleSheetCollection::activeLoadingStyleSheetLoaded(const std::vector<GCRefPtr<CSSStyleSheet>> &newStyleSheets)
{
    // StyleSheets of <style> elements that @import stylesheets are active but loading. We need to trigger a full recalc when such loads are done.
    bool hasActiveLoadingStylesheet = false;
    unsigned newStylesheetCount = newStyleSheets.size();
    for (unsigned i = 0; i < newStylesheetCount; ++i) {
        if (newStyleSheets[i]->isLoading())
            hasActiveLoadingStylesheet = true;
    }
    if (m_hadActiveLoadingStylesheet && !hasActiveLoadingStylesheet) {
        m_hadActiveLoadingStylesheet = false;
        return true;
    }
    m_hadActiveLoadingStylesheet = hasActiveLoadingStylesheet;
    return false;
}

static bool findFontFaceRulesFromStyleSheetContents(const std::vector<GCRefPtr<StyleSheetContents>> &sheets, std::vector<const StyleRuleFontFace *> &fontFaceRules)
{
    bool hasFontFaceRule = false;

    for (unsigned i = 0; i < sheets.size(); ++i) {
        ASSERT(sheets[i]);
        if (sheets[i]->hasFontFaceRule()) {
            // FIXME: We don't need this for styles in shadow tree.
            sheets[i]->findFontFaceRules(fontFaceRules);
            hasFontFaceRule = true;
        }
    }
    return hasFontFaceRule;
}

void TreeScopeStyleSheetCollection::analyzeStyleSheetChange(StyleResolverUpdateMode updateMode, const StyleSheetCollection& newCollection, StyleSheetChange& change)
{
    if (activeLoadingStyleSheetLoaded(newCollection.activeAuthorStyleSheets()))
        return;

    if (updateMode != AnalyzedStyleUpdate)
        return;

    // Find out which stylesheets are new.
    std::vector<GCRefPtr<StyleSheetContents>> addedSheets;
    if (m_activeAuthorStyleSheets.size() <= newCollection.activeAuthorStyleSheets().size()) {
        change.styleResolverUpdateType = compareStyleSheets(m_activeAuthorStyleSheets, newCollection.activeAuthorStyleSheets(), addedSheets);
    } else {
        StyleResolverUpdateType updateType = compareStyleSheets(newCollection.activeAuthorStyleSheets(), m_activeAuthorStyleSheets, addedSheets);
        if (updateType != Additive) {
            change.styleResolverUpdateType = updateType;
        } else {
            change.styleResolverUpdateType = Reset;
            // If @font-face is removed, needs full style recalc.
            if (findFontFaceRulesFromStyleSheetContents(addedSheets, change.fontFaceRulesToRemove))
                return;
        }
    }

    // FIXME: If styleResolverUpdateType is Reconstruct, we should return early here since
    // we need to recalc the whole document. It's wrong to use StyleSheetInvalidationAnalysis since
    // it only looks at the addedSheets.

    // No point in doing the analysis work if we're just going to recalc the whole document anyways.
    // This needs to be done after the compareStyleSheets calls above to ensure we don't throw away
    // the StyleResolver if we don't need to.
    if (document().hasPendingForcedStyleRecalc())
        return;

    // If we are already parsing the body and so may have significant amount of elements, put some effort into trying to avoid style recalcs.
    if (!document().body() || document().hasNodesWithPlaceholderStyle())
        return;
    StyleSheetInvalidationAnalysis invalidationAnalysis(m_treeScope, addedSheets);
    if (invalidationAnalysis.dirtiesAllStyle())
        return;
    invalidationAnalysis.invalidateStyle();
    change.requiresFullStyleRecalc = false;
    return;
}

void TreeScopeStyleSheetCollection::clearMediaQueryRuleSetStyleSheets()
{
    for (size_t i = 0; i < m_activeAuthorStyleSheets.size(); ++i) {
        StyleSheetContents* contents = m_activeAuthorStyleSheets[i]->contents();
        if (contents->hasMediaQueries())
            contents->clearRuleSet();
    }
}

DEFINE_TRACE(TreeScopeStyleSheetCollection)
{
#if 0 // BKTODO:
    visitor->trace(m_treeScope);
    visitor->trace(m_styleSheetCandidateNodes);
#endif
    StyleSheetCollection::trace(visitor);
}

TreeScopeStyleSheetCollection::StyleSheetChange::StyleSheetChange(void)
    : styleResolverUpdateType(Reconstruct)
    , requiresFullStyleRecalc(true)
{
}

TreeScopeStyleSheetCollection::StyleSheetChange::~StyleSheetChange(void) = default;

}
