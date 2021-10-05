// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: StyleSheetContents.h
// Description: StyleSheetContents Class
//      Author: Ziming Li
//     Created: 2021-07-20
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * (C) 1999-2003 Lars Knoll (knoll@kde.org)
 * Copyright (C) 2004, 2006, 2007, 2008, 2009, 2010, 2012 Apple Inc. All rights reserved.
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

#ifndef StyleSheetContents_h
#define StyleSheetContents_h

#include "core/CoreExport.h"
#include "core/css/RuleSet.h"
#include "platform/heap/Handle.h"
#include "platform/weborigin/KURL.h"
#include "wtf/HashMap.h"
#include "wtf/ListHashSet.h"
#include "wtf/RefCounted.h"
// BKTODO: #include "wtf/Vector.h"
#include "wtf/text/AtomicStringHash.h"
#include "wtf/text/StringHash.h"
#include "wtf/text/TextPosition.h"


namespace blink {

class CSSStyleSheet;
class CSSStyleSheetResource;
class Document;
class Node;
// BKTODO: class SecurityOrigin;
class StyleRuleBase;
class StyleRuleFontFace;
class StyleRuleImport;
class StyleRuleNamespace;

class CORE_EXPORT StyleSheetContents : public BlinKit::GCObject {
public:
    static PassRefPtrWillBeRawPtr<StyleSheetContents> create(const CSSParserContext& context)
    {
        return adoptRefWillBeNoop(new StyleSheetContents(0, String(), context));
    }
    static PassRefPtrWillBeRawPtr<StyleSheetContents> create(const String& originalURL, const CSSParserContext& context)
    {
        return adoptRefWillBeNoop(new StyleSheetContents(0, originalURL, context));
    }
    static PassRefPtrWillBeRawPtr<StyleSheetContents> create(StyleRuleImport* ownerRule, const String& originalURL, const CSSParserContext& context)
    {
        return adoptRefWillBeNoop(new StyleSheetContents(ownerRule, originalURL, context));
    }

    ~StyleSheetContents();

    const CSSParserContext& parserContext() const { return m_parserContext; }

    const AtomicString& defaultNamespace() { return m_defaultNamespace; }
    const AtomicString& determineNamespace(const AtomicString& prefix);

    void parseAuthorStyleSheet(const CSSStyleSheetResource*);
    void parseString(const String&);
    void parseStringAtPosition(const String&, const TextPosition&);

    bool isCacheable() const;

    bool isLoading() const;

    void checkLoaded();
    void startLoadingDynamicSheet();

    StyleSheetContents* rootStyleSheet() const;
    bool hasSingleOwnerNode() const;
    Node* singleOwnerNode() const;
    Document* singleOwnerDocument() const;

    const String& charset() const { return m_parserContext.charset(); }

    bool loadCompleted() const;
    bool hasFailedOrCanceledSubresources() const;

    void setHasSyntacticallyValidCSSHeader(bool isValidCss);
    bool hasSyntacticallyValidCSSHeader() const { return m_hasSyntacticallyValidCSSHeader; }

    void setHasFontFaceRule(bool b) { m_hasFontFaceRule = b; }
    bool hasFontFaceRule() const { return m_hasFontFaceRule; }
    void findFontFaceRules(std::vector<const StyleRuleFontFace *> &fontFaceRules);

    void parserAddNamespace(const AtomicString& prefix, const AtomicString& uri);
    void parserAppendRule(PassRefPtrWillBeRawPtr<StyleRuleBase>);

    void clearRules();

    // Rules other than @import.
    const std::vector<GCRefPtr<StyleRuleBase>>& childRules() const { return m_childRules; }
    const std::vector<GCRefPtr<StyleRuleImport>>& importRules() const { return m_importRules; }
    const std::vector<GCRefPtr<StyleRuleNamespace>>& namespaceRules() const { return m_namespaceRules; }

    void notifyLoadedSheet(const CSSStyleSheetResource*);

    StyleSheetContents* parentStyleSheet() const;
    StyleRuleImport* ownerRule(void) const;
    void clearOwnerRule(void);

    // Note that href is the URL that started the redirect chain that led to
    // this style sheet. This property probably isn't useful for much except
    // the JavaScript binding (which needs to use this value for security).
    String originalURL() const { return m_originalURL; }
    const KURL& baseURL() const { return m_parserContext.baseURL(); }

    unsigned ruleCount() const;
    StyleRuleBase* ruleAt(unsigned index) const;

    unsigned estimatedSizeInBytes() const;

    bool wrapperInsertRule(PassRefPtrWillBeRawPtr<StyleRuleBase>, unsigned index);
    bool wrapperDeleteRule(unsigned index);

    PassRefPtrWillBeRawPtr<StyleSheetContents> copy() const
    {
        return adoptRefWillBeNoop(new StyleSheetContents(*this));
    }

    void registerClient(CSSStyleSheet*);
    void unregisterClient(CSSStyleSheet*);
    size_t clientSize() const { return m_loadingClients.size() + m_completedClients.size(); }
    bool hasOneClient() { return clientSize() == 1; }
    void clientLoadCompleted(CSSStyleSheet*);
    void clientLoadStarted(CSSStyleSheet*);

    bool isMutable() const { return m_isMutable; }
    void setMutable() { m_isMutable = true; }

    void removeSheetFromCache(Document*);

    bool isInMemoryCache() const { return m_isInMemoryCache; }
    void addedToMemoryCache();
    void removedFromMemoryCache();

    void setHasMediaQueries();
    bool hasMediaQueries() const { return m_hasMediaQueries; }

    bool didLoadErrorOccur() const { return m_didLoadErrorOccur; }

    RuleSet& ruleSet() { ASSERT(m_ruleSet); return *m_ruleSet.get(); }
    RuleSet& ensureRuleSet(const MediaQueryEvaluator&, AddRuleFlags);
    void clearRuleSet();

    String sourceMapURL() const { return m_sourceMapURL; }

    DECLARE_TRACE();

private:
    StyleSheetContents(StyleRuleImport* ownerRule, const String& originalURL, const CSSParserContext&);
    StyleSheetContents(const StyleSheetContents&);
    StyleSheetContents() = delete;
    StyleSheetContents& operator=(const StyleSheetContents&) = delete;
    void notifyRemoveFontFaceRule(const StyleRuleFontFace*);

    Document* clientSingleOwnerDocument() const;

    GCRefPtr<StyleRuleImport> m_ownerRule;

    String m_originalURL;

    std::vector<GCRefPtr<StyleRuleImport>> m_importRules;
    std::vector<GCRefPtr<StyleRuleNamespace>> m_namespaceRules;
    std::vector<GCRefPtr<StyleRuleBase>> m_childRules;
    using PrefixNamespaceURIMap = std::unordered_map<AtomicString, AtomicString>;
    PrefixNamespaceURIMap m_namespaces;
    AtomicString m_defaultNamespace;

    bool m_hasSyntacticallyValidCSSHeader : 1;
    bool m_didLoadErrorOccur : 1;
    bool m_isMutable : 1;
    bool m_isInMemoryCache : 1;
    bool m_hasFontFaceRule : 1;
    bool m_hasMediaQueries : 1;
    bool m_hasSingleOwnerDocument : 1;

    CSSParserContext m_parserContext;

    std::unordered_set<CSSStyleSheet *> m_loadingClients;
    std::unordered_set<CSSStyleSheet *> m_completedClients;

    GCRefPtr<RuleSet> m_ruleSet;
    String m_sourceMapURL;
};

} // namespace

#endif
