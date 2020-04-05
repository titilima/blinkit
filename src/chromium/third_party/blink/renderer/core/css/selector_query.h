// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: selector_query.h
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

#ifndef BLINKIT_BLINK_SELECTOR_QUERY_H
#define BLINKIT_BLINK_SELECTOR_QUERY_H

#pragma once

#include <unordered_map>
#include <vector>
#include "base/macros.h"
#include "third_party/blink/renderer/core/css/css_selector_list.h"
#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/wtf/text/atomic_string.h"

namespace blink {

class ContainerNode;
class Document;
class Element;
class ExceptionState;
template <typename NodeType>
class StaticNodeTypeList;
using StaticElementList = StaticNodeTypeList<Element>;

class SelectorQuery
{
public:
    static std::unique_ptr<SelectorQuery> Adopt(CSSSelectorList selectorList);

    // https://dom.spec.whatwg.org/#dom-parentnode-queryselectorall
    StaticElementList* QueryAll(ContainerNode &rootNode) const;
private:
    explicit SelectorQuery(CSSSelectorList selectorList);

    template <typename SelectorQueryTrait>
    void Execute(ContainerNode &rootNode, typename SelectorQueryTrait::OutputType &output) const;
    template <typename SelectorQueryTrait>
    void ExecuteForTraverseRoot(ContainerNode &traverseRoot, ContainerNode &rootNode, typename SelectorQueryTrait::OutputType &output) const;
    template <typename SelectorQueryTrait>
    void FindTraverseRootsAndExecute(ContainerNode &rootNode, typename SelectorQueryTrait::OutputType &output) const;

    CSSSelectorList m_selectorList;
    // Contains the list of CSSSelector's to match, but without ones that could
    // never match like pseudo elements, div::before. This can be empty, while
    // m_selectorList will never be empty as SelectorQueryCache::add would have
    // thrown an exception.
    std::vector<const CSSSelector *> m_selectors;
    AtomicString m_selectorId;
    bool m_selectorIdIsRightmost : 1;
    bool m_selectorIdAffectedBySiblingCombinator : 1;
    bool m_usesDeepCombinatorOrShadowPseudo : 1;
    bool m_needsUpdatedDistribution : 1;
    bool m_useSlowScan : 1;
    DISALLOW_COPY_AND_ASSIGN(SelectorQuery);
};

class SelectorQueryCache
{
public:
    SelectorQuery* Add(const AtomicString &selectors, const Document &document, ExceptionState &exceptionState);
    void Invalidate(void);
private:
    std::unordered_map<AtomicString, std::unique_ptr<SelectorQuery>> m_entries;
};

} // namespace blink

#endif // BLINKIT_BLINK_SELECTOR_QUERY_H
