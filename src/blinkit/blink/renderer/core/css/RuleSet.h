// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: RuleSet.h
// Description: RuleSet Class
//      Author: Ziming Li
//     Created: 2021-07-10
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 * Copyright (C) 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011 Apple Inc. All rights reserved.
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

#ifndef RuleSet_h
#define RuleSet_h

#include "blinkit/gc/gc_linked_stack.h"
#include "core/CoreExport.h"
#include "core/css/CSSKeyframesRule.h"
#include "core/css/MediaQueryEvaluator.h"
#include "core/css/RuleFeature.h"
#include "core/css/StyleRule.h"
#include "core/css/resolver/MediaQueryResult.h"
// BKTODO: #include "platform/heap/HeapLinkedStack.h"
#include "platform/heap/HeapTerminatedArray.h"
#include "wtf/Forward.h"
#include "wtf/HashMap.h"
#include "wtf/LinkedStack.h"
#include "wtf/TerminatedArray.h"

namespace blink {

enum AddRuleFlags {
    RuleHasNoSpecialState         = 0,
    RuleHasDocumentSecurityOrigin = 1,
};

enum PropertyWhitelistType {
    PropertyWhitelistNone,
    PropertyWhitelistCue,
    PropertyWhitelistFirstLetter,
};

class CSSSelector;
class MediaQueryEvaluator;
class StyleSheetContents;

class MinimalRuleData {
    DISALLOW_NEW_EXCEPT_PLACEMENT_NEW();
public:
    MinimalRuleData(StyleRule* rule, unsigned selectorIndex, AddRuleFlags flags)
    : m_rule(rule)
    , m_selectorIndex(selectorIndex)
    , m_flags(flags)
    {
    }

    DECLARE_TRACE();

    GCRefPtr<StyleRule> m_rule;
    unsigned m_selectorIndex;
    AddRuleFlags m_flags;
};

class CORE_EXPORT RuleData {
    DISALLOW_NEW_EXCEPT_PLACEMENT_NEW();
public:
    RuleData(StyleRule*, unsigned selectorIndex, unsigned position, AddRuleFlags);

    unsigned position() const { return m_position; }
    StyleRule* rule() const { return m_rule.get(); }
    const CSSSelector& selector() const { return m_rule->selectorList().selectorAt(m_selectorIndex); }
    unsigned selectorIndex() const { return m_selectorIndex; }

    bool isLastInArray() const { return m_isLastInArray; }
    void setLastInArray(bool flag) { m_isLastInArray = flag; }

    bool containsUncommonAttributeSelector() const { return m_containsUncommonAttributeSelector; }
    unsigned specificity() const { return m_specificity; }
    unsigned linkMatchType() const { return m_linkMatchType; }
    bool hasDocumentSecurityOrigin() const { return m_hasDocumentSecurityOrigin; }
    PropertyWhitelistType propertyWhitelistType(bool isMatchingUARules = false) const { return isMatchingUARules ? PropertyWhitelistNone : static_cast<PropertyWhitelistType>(m_propertyWhitelistType); }
    // Try to balance between memory usage (there can be lots of RuleData objects) and good filtering performance.
    static const unsigned maximumIdentifierCount = 4;
    const unsigned* descendantSelectorIdentifierHashes() const { return m_descendantSelectorIdentifierHashes; }

    DECLARE_TRACE();

private:
    GCRefPtr<StyleRule> m_rule;
    unsigned m_selectorIndex : 13;
    unsigned m_isLastInArray : 1; // We store an array of RuleData objects in a primitive array.
    // This number was picked fairly arbitrarily. We can probably lower it if we need to.
    // Some simple testing showed <100,000 RuleData's on large sites.
    unsigned m_position : 18;
    unsigned m_specificity : 24;
    unsigned m_containsUncommonAttributeSelector : 1;
    unsigned m_linkMatchType : 2; //  CSSSelector::LinkMatchMask
    unsigned m_hasDocumentSecurityOrigin : 1;
    unsigned m_propertyWhitelistType : 2;
    // Use plain array instead of a Vector to minimize memory overhead.
    unsigned m_descendantSelectorIdentifierHashes[maximumIdentifierCount];
};

struct SameSizeAsRuleData {
    DISALLOW_NEW();
    RawPtrWillBeMember<void*> a;
    unsigned b;
    unsigned c;
    unsigned d[4];
};

static_assert(sizeof(RuleData) == sizeof(SameSizeAsRuleData), "RuleData should stay small");

class CORE_EXPORT RuleSet : public BlinKit::GCObject {
    WTF_MAKE_NONCOPYABLE(RuleSet);
    USING_FAST_MALLOC_WILL_BE_REMOVED(RuleSet);
public:
    static PassOwnPtrWillBeRawPtr<RuleSet> create() { return adoptPtrWillBeNoop(new RuleSet); }

    void addRulesFromSheet(StyleSheetContents*, const MediaQueryEvaluator&, AddRuleFlags = RuleHasNoSpecialState);
    void addStyleRule(StyleRule*, AddRuleFlags);
    void addRule(StyleRule*, unsigned selectorIndex, AddRuleFlags);

    const RuleFeatureSet& features() const { return m_features; }

    using RulesArray = std::vector<RuleData>;
    const RulesArray* idRules(const AtomicString& key) const { return getRules(m_idRules, key); }
    const RulesArray* classRules(const AtomicString& key) const { return getRules(m_classRules, key); }
    const RulesArray* tagRules(const AtomicString& key) const { return getRules(m_tagRules, key); }
    const RulesArray* shadowPseudoElementRules(const AtomicString& key) const { return getRules(m_shadowPseudoElementRules, key); }
    const WillBeHeapVector<RuleData>* linkPseudoClassRules() const { ASSERT(!m_pendingRules); return &m_linkPseudoClassRules; }
    const WillBeHeapVector<RuleData>* cuePseudoRules() const { ASSERT(!m_pendingRules); return &m_cuePseudoRules; }
    const WillBeHeapVector<RuleData>* focusPseudoClassRules() const { ASSERT(!m_pendingRules); return &m_focusPseudoClassRules; }
    const WillBeHeapVector<RuleData>* universalRules() const { ASSERT(!m_pendingRules); return &m_universalRules; }
    const WillBeHeapVector<RuleData>* shadowHostRules() const { ASSERT(!m_pendingRules); return &m_shadowHostRules; }
    const std::vector<GCRefPtr<StyleRulePage>>& pageRules() const { ASSERT(!m_pendingRules); return m_pageRules; }
    const WillBeHeapVector<RawPtrWillBeMember<StyleRuleViewport>>& viewportRules() const { ASSERT(!m_pendingRules); return m_viewportRules; }
    const std::vector<GCRefPtr<StyleRuleFontFace>>& fontFaceRules() const { return m_fontFaceRules; }
    const std::vector<GCRefPtr<StyleRuleKeyframes>>& keyframesRules() const { return m_keyframesRules; }
    const WillBeHeapVector<MinimalRuleData>& deepCombinatorOrShadowPseudoRules() const { return m_deepCombinatorOrShadowPseudoRules; }
    const WillBeHeapVector<MinimalRuleData>& shadowDistributedRules() const { return m_shadowDistributedRules; }
    const MediaQueryResultList& viewportDependentMediaQueryResults() const { return m_viewportDependentMediaQueryResults; }
    const MediaQueryResultList& deviceDependentMediaQueryResults() const { return m_deviceDependentMediaQueryResults; }

    unsigned ruleCount() const { return m_ruleCount; }

    void compactRulesIfNeeded()
    {
        if (!m_pendingRules)
            return;
        compactRules();
    }

#ifndef NDEBUG
    void show() const;
#endif

    DECLARE_TRACE();

private:
    using PendingRuleMap = std::unordered_map<AtomicString, BlinKit::GCLinkedStack<RuleData>>;
    using CompactRuleMap = std::unordered_map<AtomicString, RulesArray>;

    RuleSet()
        : m_ruleCount(0)
    {
    }

    void addToRuleSet(const AtomicString& key, PendingRuleMap&, const RuleData&);
    void addPageRule(StyleRulePage*);
    void addViewportRule(StyleRuleViewport*);
    void addFontFaceRule(StyleRuleFontFace*);
    void addKeyframesRule(StyleRuleKeyframes*);

    void addChildRules(const std::vector<GCRefPtr<StyleRuleBase>>&, const MediaQueryEvaluator& medium, AddRuleFlags);
    bool findBestRuleSetAndAdd(const CSSSelector&, RuleData&);

    const RulesArray* getRules(const CompactRuleMap &rules, const AtomicString &key) const
    {
        ASSERT(!m_pendingRules);
        return zed::find_value(rules, key);
    }

    void compactRules();
    static void compactPendingRules(PendingRuleMap&, CompactRuleMap&);

    class PendingRuleMaps {
    public:
        static GCUniquePtr<PendingRuleMaps> create() { return BlinKit::GCWrapUnique(new PendingRuleMaps); }

        PendingRuleMap idRules;
        PendingRuleMap classRules;
        PendingRuleMap tagRules;
        PendingRuleMap shadowPseudoElementRules;

        DECLARE_TRACE();

    private:
        PendingRuleMaps() { }
    };

    PendingRuleMaps* ensurePendingRules()
    {
        if (!m_pendingRules)
            m_pendingRules = PendingRuleMaps::create();
        return m_pendingRules.get();
    }

    CompactRuleMap m_idRules;
    CompactRuleMap m_classRules;
    CompactRuleMap m_tagRules;
    CompactRuleMap m_shadowPseudoElementRules;
    WillBeHeapVector<RuleData> m_linkPseudoClassRules;
    WillBeHeapVector<RuleData> m_cuePseudoRules;
    WillBeHeapVector<RuleData> m_focusPseudoClassRules;
    WillBeHeapVector<RuleData> m_universalRules;
    WillBeHeapVector<RuleData> m_shadowHostRules;
    RuleFeatureSet m_features;
    std::vector<GCRefPtr<StyleRulePage>> m_pageRules;
    WillBeHeapVector<RawPtrWillBeMember<StyleRuleViewport>> m_viewportRules;
    std::vector<GCRefPtr<StyleRuleFontFace>> m_fontFaceRules;
    std::vector<GCRefPtr<StyleRuleKeyframes>> m_keyframesRules;
    WillBeHeapVector<MinimalRuleData> m_deepCombinatorOrShadowPseudoRules;
    // TODO(kochi): "shadowDistributed" means the selector has ::content pseudo element.
    // Once ::slotted is introduced, come up with more readable name.
    WillBeHeapVector<MinimalRuleData> m_shadowDistributedRules;

    MediaQueryResultList m_viewportDependentMediaQueryResults;
    MediaQueryResultList m_deviceDependentMediaQueryResults;

    unsigned m_ruleCount;
    GCUniquePtr<PendingRuleMaps> m_pendingRules;

#ifndef NDEBUG
    WillBeHeapVector<RuleData> m_allRules;
#endif
};

} // namespace blink

WTF_ALLOW_MOVE_AND_INIT_WITH_MEM_FUNCTIONS(blink::RuleData);
WTF_ALLOW_MOVE_AND_INIT_WITH_MEM_FUNCTIONS(blink::MinimalRuleData);

#endif // RuleSet_h
