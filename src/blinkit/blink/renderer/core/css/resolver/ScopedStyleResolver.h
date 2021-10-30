// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: ScopedStyleResolver.h
// Description: ScopedStyleResolver Class
//      Author: Ziming Li
//     Created: 2021-08-19
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 * Copyright (C) 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011 Apple Inc. All rights reserved.
 * Copyright (C) 2012 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef ScopedStyleResolver_h
#define ScopedStyleResolver_h

#include "core/css/ElementRuleCollector.h"
#include "core/css/RuleSet.h"
#include "core/dom/TreeScope.h"

namespace blink {

class PageRuleCollector;
class StyleSheetContents;
class ViewportStyleResolver;

// This class selects a ComputedStyle for a given element based on a collection of stylesheets.
class ScopedStyleResolver final {
    WTF_MAKE_NONCOPYABLE(ScopedStyleResolver);
public:
    static std::unique_ptr<ScopedStyleResolver> create(TreeScope& scope)
    {
        return zed::wrap_unique(new ScopedStyleResolver(scope));
    }

    const TreeScope& treeScope() const { return m_scope; }
    ScopedStyleResolver* parent() const;

    StyleRuleKeyframes* keyframeStylesForAnimation(const StringImpl* animationName);

    void appendCSSStyleSheet(CSSStyleSheet&, const MediaQueryEvaluator&);
    void collectMatchingAuthorRules(ElementRuleCollector&, CascadeOrder = ignoreCascadeOrder);
    void collectMatchingShadowHostRules(ElementRuleCollector&, CascadeOrder = ignoreCascadeOrder);
    void collectMatchingTreeBoundaryCrossingRules(ElementRuleCollector&, CascadeOrder);
    void matchPageRules(PageRuleCollector&);
    void collectFeaturesTo(RuleFeatureSet&, std::unordered_set<const StyleSheetContents *>& visitedSharedStyleSheetContents) const;
    void resetAuthorStyle();
    void collectViewportRulesTo(ViewportStyleResolver*) const;
    bool hasDeepOrShadowSelector() const { return m_hasDeepOrShadowSelector; }

private:
    explicit ScopedStyleResolver(TreeScope& scope)
        : m_scope(scope)
    {
    }

    void addTreeBoundaryCrossingRules(const RuleSet&, CSSStyleSheet*, unsigned sheetIndex);
    void addKeyframeRules(const RuleSet&);
    void addFontFaceRules(const RuleSet&);
    void addKeyframeStyle(const GCRefPtr<StyleRuleKeyframes>&);

    TreeScope &m_scope;

    std::vector<GCRefPtr<CSSStyleSheet>> m_authorStyleSheets;

    using KeyframesRuleMap = std::unordered_map<const StringImpl*, GCRefPtr<StyleRuleKeyframes>>;
    KeyframesRuleMap m_keyframesRuleMap;

    class RuleSubSet final {
    public:
        static std::unique_ptr<RuleSubSet> create(CSSStyleSheet *sheet, unsigned index, const GCRefPtr<RuleSet> &rules)
        {
            return zed::wrap_unique(new RuleSubSet(sheet, index, rules));
        }

        CSSStyleSheet *m_parentStyleSheet;
        unsigned m_parentIndex;
        GCRefPtr<RuleSet> m_ruleSet;

    private:
        RuleSubSet(CSSStyleSheet *sheet, unsigned index, const GCRefPtr<RuleSet> &rules)
            : m_parentStyleSheet(sheet)
            , m_parentIndex(index)
            , m_ruleSet(rules)
        {
        }
    };
    using CSSStyleSheetRuleSubSet = std::vector<std::unique_ptr<RuleSubSet>>;

    std::unique_ptr<CSSStyleSheetRuleSubSet> m_treeBoundaryCrossingRuleSet;
    bool m_hasDeepOrShadowSelector = false;
};

} // namespace blink

#endif // ScopedStyleResolver_h
