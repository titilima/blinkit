// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: tree_scope.cpp
// Description: TreeScope Class
//      Author: Ziming Li
//     Created: 2019-10-19
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2011 Google Inc. All Rights Reserved.
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

#include "tree_scope.h"

#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/dom/id_target_observer_registry.h"
#include "third_party/blink/renderer/core/dom/tree_ordered_map.h"
#include "third_party/blink/renderer/core/dom/tree_scope_adopter.h"
#include "third_party/blink/renderer/platform/bindings/script_forbidden_scope.h"
#ifndef BLINKIT_CRAWLER_ONLY
#   include "third_party/blink/renderer/core/css/resolver/scoped_style_resolver.h"
#   include "third_party/blink/renderer/core/dom/shadow_root.h"
#endif

namespace blink {

TreeScope::TreeScope(ContainerNode &rootNode, Document &document)
    : m_rootNode(&rootNode)
    , m_document(&document)
    , m_parentTreeScope(&document)
{
    ASSERT(rootNode != document);
    m_rootNode->SetTreeScope(this);
}

TreeScope::TreeScope(Document &document)
    : m_rootNode(&document)
    , m_document(&document)
    , m_idTargetObserverRegistry(IdTargetObserverRegistry::Create())
{
    m_rootNode->SetTreeScope(this);
}

TreeScope::~TreeScope(void) = default;

void TreeScope::AddElementById(const AtomicString &elementId, Element &element)
{
    if (!m_elementsById)
        m_elementsById = TreeOrderedMap::Create();
    m_elementsById->Add(elementId, element);
    m_idTargetObserverRegistry->NotifyObservers(elementId);
}

void TreeScope::AdoptIfNeeded(Node &node)
{
    // Script is forbidden to protect against event handlers firing in the middle
    // of rescoping in |didMoveToNewDocument| callbacks. See
    // https://crbug.com/605766 and https://crbug.com/606651.
    ScriptForbiddenScope forbidScript;
    ASSERT(!node.IsDocumentNode());
    TreeScopeAdopter adopter(node, *this);
    if (adopter.NeedsScopeChange())
        adopter.Execute();
}

#ifndef BLINKIT_CRAWLER_ONLY
void TreeScope::ClearScopedStyleResolver(void)
{
    m_scopedStyleResolver.reset();
}
#endif

bool TreeScope::ContainsMultipleElementsWithId(const AtomicString &id) const
{
    return m_elementsById && m_elementsById->ContainsMultiple(id);
}

#ifndef BLINKIT_CRAWLER_ONLY
ScopedStyleResolver& TreeScope::EnsureScopedStyleResolver(void)
{
    ASSERT(nullptr != this);
    if (!m_scopedStyleResolver)
        m_scopedStyleResolver.reset(ScopedStyleResolver::Create(*this));
    return *m_scopedStyleResolver;
}
#endif

const std::vector<Member<Element>>& TreeScope::GetAllElementsById(const AtomicString &id) const
{
    if (id.IsEmpty() || !m_elementsById)
        return TreeOrderedMap::EmptyElementVector();
    return m_elementsById->GetAllElementsById(id, *this);
}

Element* TreeScope::getElementById(const AtomicString &elementId) const
{
    if (elementId.IsEmpty())
        return nullptr;
    if (!m_elementsById)
        return nullptr;
    Element *element = m_elementsById->GetElementById(elementId, *this);
    if (nullptr != element && &RootNode() == &GetDocument() && GetDocument().InDOMNodeRemovedHandler())
    {
        ASSERT(false); // BKTODO:
#if 0
        if (NodeChildRemovalTracker::IsBeingRemoved(element))
            GetDocument().CountDetachingNodeAccessInDOMNodeRemovedHandler();
#endif
    }
    return element;
}

bool TreeScope::IsInclusiveOlderSiblingShadowRootOrAncestorTreeScopeOf(const TreeScope &scope) const
{
    for (const TreeScope *current = &scope; nullptr != current; current = current->ParentTreeScope())
    {
        if (current == this)
            return true;
    }
    return false;
}

void TreeScope::RemoveElementById(const AtomicString &elementId, Element &element)
{
    ASSERT(false); // BKTODO:
}

#ifndef BLINKIT_CRAWLER_ONLY
Element* TreeScope::AdjustedFocusedElement(void) const
{
    ASSERT(false); // BKTODO:
    return nullptr;
}

bool TreeScope::HasAdoptedStyleSheets(void) const
{
    return m_adoptedStyleSheets && m_adoptedStyleSheets->length() > 0;
}
#endif

}  // namespace blink
