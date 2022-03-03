#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: StyleEngine.h
// Description: StyleEngine Class
//      Author: Ziming Li
//     Created: 2021-07-22
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
 * Copyright (C) 2011 Google Inc. All rights reserved.
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

#ifndef StyleEngine_h
#define StyleEngine_h

#include "blinkit/blink/renderer/core/css/CSSFontSelectorClient.h"
#include "blinkit/blink/renderer/core/css/invalidation/StyleInvalidator.h"
#include "blinkit/blink/renderer/core/css/resolver/StyleResolver.h"
#include "blinkit/blink/renderer/core/dom/document.h"
#include "blinkit/blink/renderer/core/dom/DocumentOrderedList.h"
#include "blinkit/blink/renderer/core/dom/DocumentStyleSheetCollection.h"
#include "blinkit/blink/renderer/wtf/Allocator.h"
#include "blinkit/blink/renderer/wtf/ListHashSet.h"
#include "blinkit/blink/renderer/wtf/RefPtr.h"
#include "blinkit/blink/renderer/wtf/TemporaryChange.h"
#include "blinkit/blink/renderer/wtf/Vector.h"
#include "blinkit/blink/renderer/wtf/text/WTFString.h"

namespace blink {

class CSSFontSelector;
class CSSStyleSheet;
class Node;
class RuleFeatureSet;
class ShadowTreeStyleSheetCollection;
class StyleRuleFontFace;
class StyleSheet;
class StyleSheetContents;

class StyleEngine final : public GCObject // BKTODO: GCObject may be unnecessary.
                        , public CSSFontSelectorClient
{
    WILL_BE_USING_GARBAGE_COLLECTED_MIXIN(StyleEngine);
public:

    class IgnoringPendingStylesheet : public TemporaryChange<bool> {
        DISALLOW_NEW();
    public:
        IgnoringPendingStylesheet(StyleEngine& engine)
            : TemporaryChange<bool>(engine.m_ignorePendingStylesheets, true)
        {
        }
    };

    friend class IgnoringPendingStylesheet;

    static GCRefPtr<StyleEngine> create(Document& document) { return BlinKit::GCWrapShared(new StyleEngine(document)); }

    ~StyleEngine();

    void detachFromDocument();

    const std::vector<GCRefPtr<StyleSheet>>& styleSheetsForStyleSheetList(TreeScope&);

    const std::vector<GCRefPtr<CSSStyleSheet>>& injectedAuthorStyleSheets() const { return m_injectedAuthorStyleSheets; }

    // BKTODO: const WillBeHeapVector<RefPtrWillBeMember<CSSStyleSheet>> activeStyleSheetsForInspector() const;

    void modifiedStyleSheet(StyleSheet*);
    void addStyleSheetCandidateNode(Node*, bool createdByParser);
    void removeStyleSheetCandidateNode(Node*);
    void removeStyleSheetCandidateNode(Node*, TreeScope&);
    void modifiedStyleSheetCandidateNode(Node*);

    void injectAuthorSheet(PassRefPtrWillBeRawPtr<StyleSheetContents> authorSheet);

    void clearMediaQueryRuleSetStyleSheets();
    void updateStyleSheetsInImport(DocumentStyleSheetCollector& parentCollector);
    void updateActiveStyleSheets(StyleResolverUpdateMode);

    String preferredStylesheetSetName() const { return m_preferredStylesheetSetName; }
    String selectedStylesheetSetName() const { return m_selectedStylesheetSetName; }
    void setPreferredStylesheetSetName(const String& name) { m_preferredStylesheetSetName = name; }
    void setSelectedStylesheetSetName(const String& name) { m_selectedStylesheetSetName = name; }

    void selectStylesheetSetName(const String& name)
    {
        setPreferredStylesheetSetName(name);
        setSelectedStylesheetSetName(name);
    }

    void addPendingSheet();
    void removePendingSheet(Node* styleSheetCandidateNode);

    bool hasPendingSheets() const { return m_pendingStylesheets > 0; }
    bool haveStylesheetsLoaded() const { return !hasPendingSheets() || m_ignorePendingStylesheets; }
    bool ignoringPendingStylesheets() const { return m_ignorePendingStylesheets; }

    unsigned maxDirectAdjacentSelectors() const { return m_maxDirectAdjacentSelectors; }
    bool usesSiblingRules() const { return m_usesSiblingRules; }
    bool usesFirstLineRules() const { return m_usesFirstLineRules; }
    bool usesWindowInactiveSelector() const { return m_usesWindowInactiveSelector; }

    bool usesRemUnits() const { return m_usesRemUnits; }
    void setUsesRemUnit(bool b) { m_usesRemUnits = b; }

    void combineCSSFeatureFlags(const RuleFeatureSet&);
    void resetCSSFeatureFlags(const RuleFeatureSet&);

    void didRemoveShadowRoot(ShadowRoot*);
    void shadowRootRemovedFromDocument(ShadowRoot*);
    void appendActiveAuthorStyleSheets();

    StyleResolver* resolver() const
    {
        return m_resolver.get();
    }

    StyleResolver& ensureResolver()
    {
        if (!m_resolver) {
            createResolver();
        } else if (m_resolver->hasPendingAuthorStyleSheets()) {
            m_resolver->appendPendingAuthorStyleSheets();
        }
        return *m_resolver.get();
    }

    bool hasResolver() const { return m_resolver.get(); }
    void clearResolver();
    void clearMasterResolver();

    StyleInvalidator& styleInvalidator() { return m_styleInvalidator; }

    CSSFontSelector* fontSelector(void);
    void setFontSelector(PassRefPtrWillBeRawPtr<CSSFontSelector>);

    void removeFontFaceRules(const std::vector<const StyleRuleFontFace *>&);
    void clearFontCache();
    // updateGenericFontFamilySettings is used from WebSettingsImpl.
    void updateGenericFontFamilySettings();

    void didDetach();
    bool shouldClearResolver() const;
    void resolverChanged(StyleResolverUpdateMode);
    unsigned resolverAccessCount() const;

    void markDocumentDirty();

    PassRefPtrWillBeRawPtr<CSSStyleSheet> createSheet(Element*, const String& text, TextPosition startPosition);
    void removeSheet(StyleSheetContents*);

    void collectScopedStyleFeaturesTo(RuleFeatureSet&) const;

    void platformColorsChanged();

    void classChangedForElement(const SpaceSplitString& changedClasses, Element&);
    void classChangedForElement(const SpaceSplitString& oldClasses, const SpaceSplitString& newClasses, Element&);
    void attributeChangedForElement(const QualifiedName& attributeName, Element&);
    void idChangedForElement(const AtomicString& oldId, const AtomicString& newId, Element&);
    void pseudoStateChangedForElement(CSSSelector::PseudoType, Element&);

    DECLARE_VIRTUAL_TRACE();

private:
    // CSSFontSelectorClient implementation.
    void fontsNeedUpdate(CSSFontSelector*) override;

private:
    StyleEngine(Document&);

    TreeScopeStyleSheetCollection* ensureStyleSheetCollectionFor(TreeScope&);
    TreeScopeStyleSheetCollection* styleSheetCollectionFor(TreeScope&);
    bool shouldUpdateDocumentStyleSheetCollection(StyleResolverUpdateMode) const;
    bool shouldUpdateShadowTreeStyleSheetCollection(StyleResolverUpdateMode) const;

    void markTreeScopeDirty(TreeScope&);

    bool isMaster() const { return m_isMaster; }
    Document* master();
    Document& document() const { return m_document; }

    typedef WillBeHeapHashSet<RawPtrWillBeMember<TreeScope>> UnorderedTreeScopeSet;

    void clearMediaQueryRuleSetOnTreeScopeStyleSheets(UnorderedTreeScopeSet&);

    void createResolver();

    static PassRefPtrWillBeRawPtr<CSSStyleSheet> parseSheet(Element*, const String& text, TextPosition startPosition);

    const DocumentStyleSheetCollection* documentStyleSheetCollection() const
    {
        return m_documentStyleSheetCollection.get();
    }

    DocumentStyleSheetCollection* documentStyleSheetCollection()
    {
        return m_documentStyleSheetCollection.get();
    }

    void updateActiveStyleSheetsInShadow(StyleResolverUpdateMode, TreeScope*, UnorderedTreeScopeSet& treeScopesRemoved);

    bool shouldSkipInvalidationFor(const Element&) const;

    Document &m_document;
    bool m_isMaster;

    // Track the number of currently loading top-level stylesheets needed for layout.
    // Sheets loaded using the @import directive are not included in this count.
    // We use this count of pending sheets to detect when we can begin attaching
    // elements and when it is safe to execute scripts.
    int m_pendingStylesheets;

    std::vector<GCRefPtr<CSSStyleSheet>> m_injectedAuthorStyleSheets; // BKTODO: Check if necessary.

    GCUniquePtr<DocumentStyleSheetCollection> m_documentStyleSheetCollection;

    using StyleSheetCollectionMap = std::unordered_map<WeakMember<TreeScope>, Member<ShadowTreeStyleSheetCollection>>;
    StyleSheetCollectionMap m_styleSheetCollectionMap;

    bool m_documentScopeDirty;
    UnorderedTreeScopeSet m_dirtyTreeScopes;
    UnorderedTreeScopeSet m_activeTreeScopes;

    String m_preferredStylesheetSetName;
    String m_selectedStylesheetSetName;

    bool m_usesSiblingRules;
    bool m_usesFirstLineRules;
    bool m_usesWindowInactiveSelector;
    bool m_usesRemUnits;
    unsigned m_maxDirectAdjacentSelectors;

    bool m_ignorePendingStylesheets;
    bool m_didCalculateResolver;
    std::unique_ptr<StyleResolver> m_resolver;
    StyleInvalidator m_styleInvalidator;

    GCRefPtr<CSSFontSelector> m_fontSelector;

    std::unordered_map<AtomicString, GCRefPtr<StyleSheetContents>> m_textToSheetCache;
    std::unordered_map<StyleSheetContents *, AtomicString> m_sheetToTextCache;
};

}

#endif
