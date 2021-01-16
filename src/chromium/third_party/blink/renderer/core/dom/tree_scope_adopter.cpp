// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: tree_scope_adopter.cpp
// Description: TreeScopeAdopter Class
//      Author: Ziming Li
//     Created: 2019-11-12
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2001 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011 Apple Inc. All
 * rights reserved.
 * Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies)
 * Copyright (C) 2009 Torch Mobile Inc. All rights reserved.
 * (http://www.torchmobile.com/)
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
 */

#include "tree_scope_adopter.h"

#include "third_party/blink/renderer/core/dom/node_traversal.h"
#ifdef BLINKIT_CRAWLER_ONLY
#   include "third_party/blink/renderer/core/dom/element.h"
#else
#   include "third_party/blink/renderer/core/dom/shadow_root.h"
#endif

namespace blink {

TreeScopeAdopter::TreeScopeAdopter(Node &toAdopt, TreeScope &newScope)
    : m_toAdopt(toAdopt), m_newScope(newScope), m_oldScope(toAdopt.GetTreeScope())
{
}

void TreeScopeAdopter::Execute(void) const
{
    MoveTreeToNewScope(*m_toAdopt);
    Document &oldDocument = OldScope().GetDocument();
    if (oldDocument == NewScope().GetDocument())
        return;
    ASSERT(false); // BKTODO: oldDocument.DidMoveTreeToNewDocument(*m_toAdopt);
}

void TreeScopeAdopter::MoveTreeToNewScope(Node &root) const
{
    ASSERT(NeedsScopeChange());

    // If an element is moved from a document and then eventually back again the
    // collection cache for that element may contain stale data as changes made to
    // it will have updated the DOMTreeVersion of the document it was moved to. By
    // increasing the DOMTreeVersion of the donating document here we ensure that
    // the collection cache will be invalidated as needed when the element is
    // moved back.
    Document &oldDocument = OldScope().GetDocument();
    Document &newDocument = NewScope().GetDocument();

    bool willMoveToNewDocument = oldDocument != newDocument;
    for (Node &node : NodeTraversal::InclusiveDescendantsOf(root))
    {
        UpdateTreeScope(node);

        if (willMoveToNewDocument)
        {
            ASSERT(false); // BKTODO: MoveNodeToNewDocument(node, old_document, new_document);
        }
        else if (node.HasRareData())
        {
            NodeRareData *rareData = node.RareData();
            if (rareData->NodeLists())
                ASSERT(false); // BKTODO: rareData->NodeLists()->AdoptTreeScope();
        }

        if (!node.IsElementNode())
            continue;

        Element &element = ToElement(node);
        if (auto *attrs = element.GetAttrNodeList())
        {
            ASSERT(false); // BKTODO:
#if 0
            for (const auto& attr : *attrs)
                MoveTreeToNewScope(*attr);
#endif
        }

#ifndef BLINKIT_CRAWLER_ONLY
        if (ShadowRoot *shadow = element.GetShadowRoot())
        {
            shadow->SetParentTreeScope(NewScope());
            if (willMoveToNewDocument)
                ASSERT(false); // BKTODO: MoveShadowTreeToNewDocument(*shadow, old_document, new_document);
        }
#endif
    }
}

void TreeScopeAdopter::UpdateTreeScope(Node &node) const
{
    ASSERT(!node.IsTreeScope());
    ASSERT(node.GetTreeScope() == OldScope());
    node.SetTreeScope(m_newScope);
}

} // namespace blink
