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

#ifndef StyleResolver_h
#define StyleResolver_h

#include "core/CoreExport.h"
#include "core/animation/PropertyHandle.h"
#include "core/css/ElementRuleCollector.h"
#include "core/css/PseudoStyleRequest.h"
#include "core/css/RuleFeature.h"
#include "core/css/RuleSet.h"
#include "core/css/SelectorChecker.h"
#include "core/css/SelectorFilter.h"
#include "core/css/resolver/CSSPropertyPriority.h"
#include "core/css/resolver/MatchedPropertiesCache.h"
#include "core/css/resolver/StyleBuilder.h"
#include "core/css/resolver/StyleResolverStats.h"
#include "core/dom/DocumentOrderedList.h"
#include "core/style/CachedUAStyle.h"
#include "platform/heap/Handle.h"
#include "wtf/Deque.h"
#include "wtf/HashMap.h"
#include "wtf/HashSet.h"
#include "wtf/ListHashSet.h"
#include "wtf/RefPtr.h"
#include "wtf/Vector.h"

namespace blink {

class AnimatableValue;
class CSSRuleList;
class CSSStyleSheet;
class CSSValue;
class ContainerNode;
class Document;
class Element;
class Interpolation;
class MatchResult;
class MediaQueryEvaluator;
class ScopedStyleResolver;
class StylePropertySet;
class StyleRule;
class ViewportStyleResolver;

enum StyleSharingBehavior {
    AllowStyleSharing,
    DisallowStyleSharing,
};

enum RuleMatchingBehavior {
    MatchAllRules,
    MatchAllRulesExcludingSMIL
};

const unsigned styleSharingListSize = 15;
const unsigned styleSharingMaxDepth = 32;
using StyleSharingList = WillBeHeapDeque<RawPtrWillBeMember<Element>, styleSharingListSize>;
using ActiveInterpolationsMap = HashMap<PropertyHandle, Vector<RefPtr<Interpolation>, 1>>;

// This class selects a ComputedStyle for a given element based on a collection of stylesheets.
class CORE_EXPORT StyleResolver final : public NoBaseWillBeGarbageCollectedFinalized<StyleResolver> {
    WTF_MAKE_NONCOPYABLE(StyleResolver); USING_FAST_MALLOC_WILL_BE_REMOVED(StyleResolver);
public:
    static PassOwnPtrWillBeRawPtr<StyleResolver> create(Document& document)
    {
        return adoptPtrWillBeNoop(new StyleResolver(document));
    }
    ~StyleResolver();
    void dispose();

    PassRefPtr<ComputedStyle> styleForElement(Element*, const ComputedStyle* parentStyle = 0, StyleSharingBehavior = AllowStyleSharing,
        RuleMatchingBehavior = MatchAllRules);

    static PassRefPtr<AnimatableValue> createAnimatableValueSnapshot(Element&, const ComputedStyle* baseStyle, CSSPropertyID, CSSValue*);
    static PassRefPtr<AnimatableValue> createAnimatableValueSnapshot(StyleResolverState&, CSSPropertyID, CSSValue*);

    PassRefPtr<ComputedStyle> pseudoStyleForElement(Element*, const PseudoStyleRequest&, const ComputedStyle* parentStyle);

    PassRefPtr<ComputedStyle> styleForPage(int pageIndex);
    PassRefPtr<ComputedStyle> styleForText(Text*);

    static PassRefPtr<ComputedStyle> styleForDocument(Document&);

    // FIXME: It could be better to call appendAuthorStyleSheets() directly after we factor StyleResolver further.
    // https://bugs.webkit.org/show_bug.cgi?id=108890
    void appendAuthorStyleSheets(const WillBeHeapVector<RefPtrWillBeMember<CSSStyleSheet>>&);
    void resetAuthorStyle(TreeScope&);
    void finishAppendAuthorStyleSheets();

    void lazyAppendAuthorStyleSheets(unsigned firstNew, const WillBeHeapVector<RefPtrWillBeMember<CSSStyleSheet>>&);
    void removePendingAuthorStyleSheets(const WillBeHeapVector<RefPtrWillBeMember<CSSStyleSheet>>&);
    void appendPendingAuthorStyleSheets();
    bool hasPendingAuthorStyleSheets() const { return m_pendingStyleSheets.size() > 0 || m_needCollectFeatures; }

    // TODO(esprehn): StyleResolver should probably not contain tree walking
    // state, instead we should pass a context object during recalcStyle.
    SelectorFilter& selectorFilter() { return m_selectorFilter; }

    StyleRuleKeyframes* findKeyframesRule(const Element*, const AtomicString& animationName);

    // These methods will give back the set of rules that matched for a given element (or a pseudo-element).
    enum CSSRuleFilter {
        UAAndUserCSSRules   = 1 << 1,
        AuthorCSSRules      = 1 << 2,
        EmptyCSSRules       = 1 << 3,
        CrossOriginCSSRules = 1 << 4,
        AllButEmptyCSSRules = UAAndUserCSSRules | AuthorCSSRules | CrossOriginCSSRules,
        AllCSSRules         = AllButEmptyCSSRules | EmptyCSSRules,
    };
    PassRefPtrWillBeRawPtr<CSSRuleList> cssRulesForElement(Element*, unsigned rulesToInclude = AllButEmptyCSSRules);
    PassRefPtrWillBeRawPtr<CSSRuleList> pseudoCSSRulesForElement(Element*, PseudoId, unsigned rulesToInclude = AllButEmptyCSSRules);
    PassRefPtrWillBeRawPtr<StyleRuleList> styleRulesForElement(Element*, unsigned rulesToInclude);

    void computeFont(ComputedStyle*, const StylePropertySet&);

    ViewportStyleResolver* viewportStyleResolver() { return m_viewportStyleResolver.get(); }

    void addViewportDependentMediaQueries(const MediaQueryResultList&);
    bool hasViewportDependentMediaQueries() const { return !m_viewportDependentMediaQueryResults.isEmpty(); }
    bool mediaQueryAffectedByViewportChange() const;
    void addDeviceDependentMediaQueries(const MediaQueryResultList&);
    bool mediaQueryAffectedByDeviceChange() const;

    // FIXME: Rename to reflect the purpose, like didChangeFontSize or something.
    void invalidateMatchedPropertiesCache();

    void notifyResizeForViewportUnits();

    // Exposed for ComputedStyle::isStyleAvilable().
    static ComputedStyle* styleNotYetAvailable() { return s_styleNotYetAvailable; }

    RuleFeatureSet& ensureUpdatedRuleFeatureSet()
    {
        if (hasPendingAuthorStyleSheets())
            appendPendingAuthorStyleSheets();
        return m_features;
    }

    RuleFeatureSet& ruleFeatureSet()
    {
        return m_features;
    }

    StyleSharingList& styleSharingList();

    bool hasRulesForId(const AtomicString&) const;

    void addToStyleSharingList(Element&);
    void clearStyleSharingList();

    StyleResolverStats* stats() { return m_styleResolverStats.get(); }
    void setStatsEnabled(bool);

    unsigned accessCount() const { return m_accessCount; }
    void didAccess() { ++m_accessCount; }

    void increaseStyleSharingDepth() { ++m_styleSharingDepth; }
    void decreaseStyleSharingDepth() { --m_styleSharingDepth; }

    PassRefPtrWillBeRawPtr<PseudoElement> createPseudoElementIfNeeded(Element& parent, PseudoId);

    DECLARE_TRACE();

    void addTreeBoundaryCrossingScope(ContainerNode& scope);

private:
    explicit StyleResolver(Document&);

    PassRefPtr<ComputedStyle> initialStyleForElement();

    void initWatchedSelectorRules();

    // FIXME: This should probably go away, folded into FontBuilder.
    void updateFont(StyleResolverState&);

    void loadPendingResources(StyleResolverState&);
    void adjustComputedStyle(StyleResolverState&, Element*);

    void appendCSSStyleSheet(CSSStyleSheet&);

    void collectPseudoRulesForElement(const Element&, ElementRuleCollector&, PseudoId, unsigned rulesToInclude);
    void matchRuleSet(ElementRuleCollector&, RuleSet*);
    void matchUARules(ElementRuleCollector&);
    void matchAuthorRules(const Element&, ElementRuleCollector&);
    void matchAllRules(StyleResolverState&, ElementRuleCollector&, bool includeSMILProperties);
    void collectFeatures();
    void collectTreeBoundaryCrossingRules(const Element&, ElementRuleCollector&);
    void resetRuleFeatures();

    void applyMatchedProperties(StyleResolverState&, const MatchResult&);
    bool applyAnimatedProperties(StyleResolverState&, const Element* animatingElement);
    void applyCallbackSelectors(StyleResolverState&);

    template <CSSPropertyPriority priority>
    void applyMatchedProperties(StyleResolverState&, const MatchedPropertiesRange&, bool important, bool inheritedOnly);
    template <CSSPropertyPriority priority>
    void applyProperties(StyleResolverState&, const StylePropertySet* properties, bool isImportant, bool inheritedOnly, PropertyWhitelistType = PropertyWhitelistNone);
    template <CSSPropertyPriority priority>
    void applyAnimatedProperties(StyleResolverState&, const ActiveInterpolationsMap&);
    template <CSSPropertyPriority priority>
    void applyAllProperty(StyleResolverState&, CSSValue*, bool inheritedOnly, PropertyWhitelistType);

    bool pseudoStyleForElementInternal(Element&, const PseudoStyleRequest&, const ComputedStyle* parentStyle, StyleResolverState&);
    bool hasAuthorBackground(const StyleResolverState&);
    bool hasAuthorBorder(const StyleResolverState&);

    PassRefPtrWillBeRawPtr<PseudoElement> createPseudoElement(Element* parent, PseudoId);

    Document& document() { return *m_document; }

    static ComputedStyle* s_styleNotYetAvailable;

    MatchedPropertiesCache m_matchedPropertiesCache;

    OwnPtrWillBeMember<MediaQueryEvaluator> m_medium;
    MediaQueryResultList m_viewportDependentMediaQueryResults;
    MediaQueryResultList m_deviceDependentMediaQueryResults;

    RawPtrWillBeMember<Document> m_document;
    SelectorFilter m_selectorFilter;

    OwnPtrWillBeMember<ViewportStyleResolver> m_viewportStyleResolver;

    WillBeHeapListHashSet<RawPtrWillBeMember<CSSStyleSheet>, 16> m_pendingStyleSheets;

    // FIXME: The entire logic of collecting features on StyleResolver, as well as transferring them
    // between various parts of machinery smells wrong. This needs to be better somehow.
    RuleFeatureSet m_features;
    OwnPtrWillBeMember<RuleSet> m_siblingRuleSet;
    OwnPtrWillBeMember<RuleSet> m_uncommonAttributeRuleSet;
    OwnPtrWillBeMember<RuleSet> m_watchedSelectorsRules;

    DocumentOrderedList m_treeBoundaryCrossingScopes;

    bool m_needCollectFeatures;
    bool m_printMediaType;

    unsigned m_styleSharingDepth;
    WillBeHeapVector<OwnPtrWillBeMember<StyleSharingList>, styleSharingMaxDepth> m_styleSharingLists;

    OwnPtr<StyleResolverStats> m_styleResolverStats;

    // Use only for Internals::updateStyleAndReturnAffectedElementCount.
    unsigned m_accessCount;
};

} // namespace blink

#endif // StyleResolver_h
