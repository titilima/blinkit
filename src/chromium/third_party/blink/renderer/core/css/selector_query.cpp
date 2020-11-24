// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: selector_query.cpp
// Description: SelectorQuery Class
//      Author: Ziming Li
//     Created: 2020-01-17
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2011, 2013 Apple Inc. All rights reserved.
 * Copyright (C) 2014 Samsung Electronics. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "selector_query.h"

#include "base/memory/ptr_util.h"
#include "third_party/blink/renderer/core/css/parser/css_parser.h"
#include "third_party/blink/renderer/core/css/selector_checker.h"
#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/dom/element_traversal.h"
#include "third_party/blink/renderer/core/dom/nth_index_cache.h"
#include "third_party/blink/renderer/core/dom/static_node_list.h"
#include "third_party/blink/renderer/core/html_names.h"
#include "third_party/blink/renderer/platform/bindings/exception_state.h"

using namespace BlinKit;

namespace blink {

using namespace html_names;

struct AllElementsSelectorQueryTrait {
    typedef std::vector<Element *> OutputType;
    static const bool kShouldOnlyMatchFirstElement = false;
    ALWAYS_INLINE static bool IsEmpty(const OutputType &output)
    {
        return output.empty();
    }
    ALWAYS_INLINE static void AppendElement(OutputType &output, Element &element)
    {
        output.push_back(&element);
    }
};

struct SingleElementSelectorQueryTrait {
    typedef Element *OutputType;
    static const bool kShouldOnlyMatchFirstElement = true;
    ALWAYS_INLINE static bool IsEmpty(const OutputType &output)
    {
        return nullptr == output;
    }
    ALWAYS_INLINE static void AppendElement(OutputType &output, Element &element)
    {
        ASSERT(nullptr == output);
        output = &element;
    }
};

static inline bool MatchesTagName(const QualifiedName &tagName, const Element &element)
{
    if (tagName == AnyQName())
        return true;
    if (element.HasLocalName(tagName.LocalName()))
        return true;
    // Non-html elements in html documents are normalized to their camel-cased
    // version during parsing if applicable. Yet, type selectors are lower-cased
    // for selectors in html documents. Compare the upper case converted names
    // instead to allow matching SVG elements like foreignObject.
    if (!element.IsHTMLElement() && element.GetDocument().IsHTMLDocument())
        return element.TagQName().LocalNameUpper() == tagName.LocalNameUpper();
    return false;
}

inline bool SelectorMatches(const CSSSelector &selector, Element &element, const ContainerNode &rootNode)
{
    SelectorChecker::Init init;
    init.mode = SelectorChecker::kQueryingRules;
    SelectorChecker checker(init);
    SelectorChecker::SelectorCheckingContext context(&element, SelectorChecker::kVisitedMatchDisabled);
    context.selector = &selector;
    context.scope = &rootNode;
    return checker.Match(context);
}

template <typename SelectorQueryTrait>
static void CollectElementsByClassName(ContainerNode &rootNode, const AtomicString &className, const CSSSelector *selector, typename SelectorQueryTrait::OutputType &output)
{
    for (Element &element : ElementTraversal::DescendantsOf(rootNode))
    {
        if (!element.HasClassName(className))
            continue;
        if (selector && !SelectorMatches(*selector, element, rootNode))
            continue;
        SelectorQueryTrait::AppendElement(output, element);
        if (SelectorQueryTrait::kShouldOnlyMatchFirstElement)
            return;
    }
}

template <typename SelectorQueryTrait>
static void CollectElementsByTagName(ContainerNode &rootNode, const QualifiedName &tagName, typename SelectorQueryTrait::OutputType &output)
{
    ASSERT(tagName.NamespaceURI() == g_star_atom);
    for (Element &element : ElementTraversal::DescendantsOf(rootNode))
    {
        if (MatchesTagName(tagName, element))
        {
            SelectorQueryTrait::AppendElement(output, element);
            if (SelectorQueryTrait::kShouldOnlyMatchFirstElement)
                return;
        }
    }
}

SelectorQuery::SelectorQuery(CSSSelectorList selectorList)
    : m_selectorList(std::move(selectorList))
    , m_selectorIdIsRightmost(true)
    , m_selectorIdAffectedBySiblingCombinator(false)
    , m_usesDeepCombinatorOrShadowPseudo(false)
    , m_needsUpdatedDistribution(false)
    , m_useSlowScan(true)
{
    m_selectors.reserve(m_selectorList.ComputeLength());
    for (const CSSSelector *selector = m_selectorList.First(); nullptr != selector; selector = CSSSelectorList::Next(*selector))
    {
        if (selector->MatchesPseudoElement())
            continue;
        m_selectors.push_back(selector);
        m_usesDeepCombinatorOrShadowPseudo |= selector->HasDeepCombinatorOrShadowPseudo();
        m_needsUpdatedDistribution |= selector->NeedsUpdatedDistribution();
    }

    if (m_selectors.size() == 1 && !m_usesDeepCombinatorOrShadowPseudo && !m_needsUpdatedDistribution)
    {
        m_useSlowScan = false;
        for (const CSSSelector *current = m_selectors[0]; nullptr != current; current = current->TagHistory())
        {
            if (current->Match() == CSSSelector::kId)
            {
                m_selectorId = current->Value();
                break;
            }
            // We only use the fast path when in standards mode where #id selectors
            // are case sensitive, so we need the same behavior for [id=value].
            if (current->Match() == CSSSelector::kAttributeExact && current->Attribute() == kIdAttr
                && current->AttributeMatch() == CSSSelector::kCaseSensitive)
            {
                m_selectorId = current->Value();
                break;
            }
            if (current->Relation() == CSSSelector::kSubSelector)
                continue;
            m_selectorIdIsRightmost = false;
            m_selectorIdAffectedBySiblingCombinator =
                current->Relation() == CSSSelector::kDirectAdjacent
                || current->Relation() == CSSSelector::kIndirectAdjacent;
        }
    }
}

std::unique_ptr<SelectorQuery> SelectorQuery::Adopt(CSSSelectorList selectorList)
{
    return base::WrapUnique(new SelectorQuery(std::move(selectorList)));
}

template <typename SelectorQueryTrait>
void SelectorQuery::Execute(ContainerNode &rootNode, typename SelectorQueryTrait::OutputType &output) const
{
    if (m_selectors.empty())
        return;

    if (m_useSlowScan)
    {
        ASSERT(false); // BKTODO:
#if 0
        if (m_needsUpdatedDistribution)
            rootNode.UpdateDistributionForFlatTreeTraversal();
        if (m_usesDeepCombinatorOrShadowPseudo)
            ExecuteSlowTraversingShadowTree<SelectorQueryTrait>(rootNode, output);
        else
            ExecuteSlow<SelectorQueryTrait>(rootNode, output);
#endif
        return;
    }

    ASSERT(m_selectors.size() == 1);
    ASSERT(!m_needsUpdatedDistribution);
    ASSERT(!m_usesDeepCombinatorOrShadowPseudo);

    // In quirks mode getElementById("a") is case sensitive and should only
    // match elements with lowercase id "a", but querySelector is case-insensitive
    // so querySelector("#a") == querySelector("#A"), which means we can only use
    // the id fast path when we're in a standards mode document.
    if (m_selectorId && rootNode.IsInTreeScope() && !rootNode.GetDocument().InQuirksMode())
    {
        ExecuteWithId<SelectorQueryTrait>(rootNode, output);
        return;
    }

    const CSSSelector &firstSelector = *m_selectors[0];
    if (!firstSelector.TagHistory())
    {
        // Fast path for querySelector*('.foo'), and querySelector*('div').
        switch (firstSelector.Match())
        {
        case CSSSelector::kClass:
            CollectElementsByClassName<SelectorQueryTrait>(rootNode, firstSelector.Value(), nullptr, output);
            return;
        case CSSSelector::kTag:
            if (firstSelector.TagQName().NamespaceURI() == g_star_atom)
            {
                CollectElementsByTagName<SelectorQueryTrait>(rootNode, firstSelector.TagQName(), output);
                return;
            }
            // querySelector*() doesn't allow namespace prefix resolution and
            // throws before we get here, but we still may have selectors for
            // elements without a namespace.
            ASSERT(firstSelector.TagQName().NamespaceURI() == g_null_atom);
            break;
        default:
            break;  // If we need another fast path, add here.
        }
    }

    FindTraverseRootsAndExecute<SelectorQueryTrait>(rootNode, output);
}

template <typename SelectorQueryTrait>
void SelectorQuery::ExecuteForTraverseRoot(ContainerNode &traverseRoot, ContainerNode &rootNode, typename SelectorQueryTrait::OutputType &output) const
{
    ASSERT(m_selectors.size() == 1);

    const CSSSelector &selector = *m_selectors[0];

    for (Element &element : ElementTraversal::DescendantsOf(traverseRoot))
    {
        if (SelectorMatches(selector, element, rootNode))
        {
            SelectorQueryTrait::AppendElement(output, element);
            if (SelectorQueryTrait::kShouldOnlyMatchFirstElement)
                return;
        }
    }
}

template <typename SelectorQueryTrait>
void SelectorQuery::ExecuteWithId(ContainerNode &rootNode, typename SelectorQueryTrait::OutputType &output) const
{
    ASSERT(m_selectors.size() == 1);
    ASSERT(!rootNode.GetDocument().InQuirksMode());

    const CSSSelector &firstSelector = *m_selectors[0];
    const TreeScope &scope = rootNode.ContainingTreeScope();

    if (scope.ContainsMultipleElementsWithId(m_selectorId))
    {
        // We don't currently handle cases where there's multiple elements with the
        // id and it's not in the rightmost selector.
        if (!m_selectorIdIsRightmost)
        {
            FindTraverseRootsAndExecute<SelectorQueryTrait>(rootNode, output);
            return;
        }
        const auto &elements = scope.GetAllElementsById(m_selectorId);
        for (const auto &element : elements)
        {
            if (!element->IsDescendantOf(&rootNode))
                continue;
            if (SelectorMatches(firstSelector, *element, rootNode))
            {
                SelectorQueryTrait::AppendElement(output, *element);
                if (SelectorQueryTrait::kShouldOnlyMatchFirstElement)
                    return;
            }
        }
        return;
    }

    Element *element = scope.getElementById(m_selectorId);
    if (nullptr == element)
        return;
    if (m_selectorIdIsRightmost)
    {
        if (!element->IsDescendantOf(&rootNode))
            return;
        if (SelectorMatches(firstSelector, *element, rootNode))
            SelectorQueryTrait::AppendElement(output, *element);
        return;
    }
    ContainerNode *start = &rootNode;
    if (element->IsDescendantOf(&rootNode))
        start = element;
    if (m_selectorIdAffectedBySiblingCombinator)
        start = start->parentNode();
    if (nullptr == start)
        return;
    ExecuteForTraverseRoot<SelectorQueryTrait>(*start, rootNode, output);
}

inline static bool AncestorHasClassName(ContainerNode &rootNode, const AtomicString &className)
{
    if (!rootNode.IsElementNode())
        return false;

    for (Element *element = &ToElement(rootNode); nullptr != element; element = element->parentElement())
    {
        if (element->HasClassName(className))
            return true;
    }
    return false;
}

template <typename SelectorQueryTrait>
void SelectorQuery::FindTraverseRootsAndExecute(ContainerNode &rootNode, typename SelectorQueryTrait::OutputType &output) const
{
    // We need to return the matches in document order. To use id lookup while
    // there is possiblity of multiple matches we would need to sort the
    // results. For now, just traverse the document in that case.
    ASSERT(m_selectors.size() == 1);

    bool isRightmostSelector = true;
    bool isAffectedBySiblingCombinator = false;

    for (const CSSSelector *selector = m_selectors[0]; nullptr != selector; selector = selector->TagHistory())
    {
        if (!isAffectedBySiblingCombinator && selector->Match() == CSSSelector::kClass)
        {
            if (isRightmostSelector)
            {
                CollectElementsByClassName<SelectorQueryTrait>(rootNode, selector->Value(), m_selectors[0], output);
                return;
            }
            // Since there exists some ancestor element which has the class name, we
            // need to see all children of rootNode.
            if (AncestorHasClassName(rootNode, selector->Value()))
                break;

            const AtomicString &className = selector->Value();
            Element *element = ElementTraversal::FirstWithin(rootNode);
            while (nullptr != element)
            {
                if (element->HasClassName(className))
                {
                    ExecuteForTraverseRoot<SelectorQueryTrait>(*element, rootNode, output);
                    if (SelectorQueryTrait::kShouldOnlyMatchFirstElement && !SelectorQueryTrait::IsEmpty(output))
                        return;
                    element = ElementTraversal::NextSkippingChildren(*element, &rootNode);
                }
                else
                {
                    element = ElementTraversal::Next(*element, &rootNode);
                }
            }
            return;
        }

        if (selector->Relation() == CSSSelector::kSubSelector)
            continue;
        isRightmostSelector = false;
        isAffectedBySiblingCombinator =
            selector->Relation() == CSSSelector::kDirectAdjacent
            || selector->Relation() == CSSSelector::kIndirectAdjacent;
    }

    ExecuteForTraverseRoot<SelectorQueryTrait>(rootNode, rootNode, output);
}

StaticElementList* SelectorQuery::QueryAll(ContainerNode &rootNode) const
{
    NthIndexCache nthIndexCache(rootNode.GetDocument());
    std::vector<Element *> result;
    Execute<AllElementsSelectorQueryTrait>(rootNode, result);
    return StaticElementList::Adopt(result);
}

Element* SelectorQuery::QueryFirst(ContainerNode &rootNode) const
{
    NthIndexCache nthIndexCache(rootNode.GetDocument());
    Element *matchedElement = nullptr;
    Execute<SingleElementSelectorQueryTrait>(rootNode, matchedElement);
    return matchedElement;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SelectorQuery* SelectorQueryCache::Add(const AtomicString &selectors, const Document &document, ExceptionState &exceptionState)
{
    if (selectors.IsEmpty())
    {
        exceptionState.ThrowDOMException(DOMExceptionCode::kSyntaxError, "The provided selector is empty.");
        return nullptr;
    }

    auto it = m_entries.find(selectors);
    if (std::end(m_entries) != it)
        return it->second.get();

    CSSParserContext *context = CSSParserContext::Create(GCObjectType::Stash, document, document.BaseURL(), false,
        WTF::TextEncoding(), CSSParserContext::kSnapshotProfile);
    CSSSelectorList selectorList = CSSParser::ParseSelector(context, nullptr, selectors);
    if (nullptr == selectorList.First())
    {
        exceptionState.ThrowDOMException(DOMExceptionCode::kSyntaxError,
            "'" + selectors + "' is not a valid selector.");
        return nullptr;
    }

    const unsigned kMaximumSelectorQueryCacheSize = 256;
    if (m_entries.size() == kMaximumSelectorQueryCacheSize)
        m_entries.erase(m_entries.begin());

    std::unique_ptr<SelectorQuery> query = SelectorQuery::Adopt(std::move(selectorList));
    SelectorQuery *ret = query.get();
    m_entries[selectors] = std::move(query);
    return ret;
}

void SelectorQueryCache::Invalidate(void)
{
    m_entries.clear();
}

} // namespace blink
