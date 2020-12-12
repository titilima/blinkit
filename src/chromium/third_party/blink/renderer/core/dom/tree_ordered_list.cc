// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: tree_ordered_list.cc
// Description: TreeOrderedList Class
//      Author: Ziming Li
//     Created: 2020-09-26
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2001 Dirk Mueller (mueller@kde.org)
 *           (C) 2006 Alexey Proskuryakov (ap@webkit.org)
 * Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009, 2011, 2012 Apple Inc. All
 * rights reserved.
 * Copyright (C) 2008, 2009 Torch Mobile Inc. All rights reserved.
 * (http://www.torchmobile.com/)
 * Copyright (C) 2008, 2009, 2011, 2012 Google Inc. All rights reserved.
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies)
 * Copyright (C) 2013 Google Inc. All rights reserved.
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

#include "third_party/blink/renderer/core/dom/tree_ordered_list.h"

#include "third_party/blink/renderer/core/dom/node.h"

namespace blink {

void TreeOrderedList::Add(Node* node) {
  ASSERT(false); // BKTODO:
#if 0
  if (nodes_.IsEmpty()) {
    nodes_.insert(node);
    return;
  }

  // Determine an appropriate insertion point.
  iterator begin = nodes_.begin();
  iterator end = nodes_.end();
  iterator it = end;
  Node* following_node = nullptr;
  do {
    --it;
    Node* n = *it;
    unsigned short position =
        n->compareDocumentPosition(node, Node::kTreatShadowTreesAsComposed);
    if (position & Node::kDocumentPositionFollowing) {
      nodes_.InsertBefore(following_node, node);
      return;
    }
    following_node = n;
  } while (it != begin);

  nodes_.InsertBefore(following_node, node);
#endif
}

void TreeOrderedList::Remove(const Node* node) {
  ASSERT(false); // BKTODO: nodes_.erase(const_cast<Node*>(node));
}

void TreeOrderedList::Trace(blink::Visitor* visitor) {
  // BKTODO: visitor->Trace(nodes_);
}

}  // namespace blink
