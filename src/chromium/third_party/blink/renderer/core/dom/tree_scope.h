// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: tree_scope.h
// Description: TreeScope Class
//      Author: Ziming Li
//     Created: 2019-10-19
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2011 Google Inc. All Rights Reserved.
 * Copyright (C) 2012 Apple Inc. All Rights Reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef BLINKIT_BLINK_TREE_SCOPE_H
#define BLINKIT_BLINK_TREE_SCOPE_H

#pragma once

#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/wtf/text/atomic_string.h"

namespace blink {

class ContainerNode;
class Document;
class Element;
class IdTargetObserverRegistry;
class Node;
class ScopedStyleResolver;
class TreeOrderedMap;
#ifndef BLINKIT_CRAWLER_ONLY
class ScopedStyleResolver;
class StyleSheetList;
#endif

class TreeScope : public GarbageCollectedMixin
{
public:
    ContainerNode& RootNode(void) const { return *m_rootNode; }
    TreeScope* ParentTreeScope(void) const { return m_parentTreeScope; }
    
    bool IsInclusiveOlderSiblingShadowRootOrAncestorTreeScopeOf(const TreeScope &scope) const;

    Element* getElementById(const AtomicString &elementId) const;
    const std::vector<Member<Element>>& GetAllElementsById(const AtomicString &id) const;
    bool ContainsMultipleElementsWithId(const AtomicString &id) const;
    void AddElementById(const AtomicString &elementId, Element &element);
    void RemoveElementById(const AtomicString &elementId, Element &element);

    Document& GetDocument(void) const
    {
        ASSERT(m_document);
        return *m_document;
    }

    // Used by the basic DOM mutation methods (e.g., appendChild()).
    void AdoptIfNeeded(Node &node);

#ifndef BLINKIT_CRAWLER_ONLY
    Element* AdjustedFocusedElement(void) const;

    ScopedStyleResolver* GetScopedStyleResolver(void) const { return m_scopedStyleResolver.Get(); }
    ScopedStyleResolver& EnsureScopedStyleResolver(void);
    void ClearScopedStyleResolver(void);

    bool HasAdoptedStyleSheets(void) const;
#endif

protected:
    TreeScope(ContainerNode &rootNode, Document &document);
    TreeScope(Document &document);
    virtual ~TreeScope(void);
    void Trace(Visitor *visitor) override;

    void SetDocument(Document &document) { m_document = &document; }
    void SetParentTreeScope(TreeScope &newParentScope);

#ifndef BLINKIT_CRAWLER_ONLY
    void SetNeedsStyleRecalcForViewportUnits(void);
#endif
private:
    Member<ContainerNode> m_rootNode;
    Member<Document> m_document;
    Member<TreeScope> m_parentTreeScope;

    std::unique_ptr<TreeOrderedMap> m_elementsById;

    std::unique_ptr<IdTargetObserverRegistry> m_idTargetObserverRegistry;

#ifndef BLINKIT_CRAWLER_ONLY
    Member<ScopedStyleResolver> m_scopedStyleResolver;
    Member<StyleSheetList> m_adoptedStyleSheets;
#endif
};

DEFINE_COMPARISON_OPERATORS_WITH_REFERENCES(TreeScope)

}  // namespace blink

#endif // BLINKIT_BLINK_TREE_SCOPE_H
