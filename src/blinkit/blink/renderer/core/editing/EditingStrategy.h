#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: EditingAlgorithm.h
// Description: EditingAlgorithm Class
//      Author: Ziming Li
//     Created: 2022-03-02
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EditingStrategy_h
#define EditingStrategy_h

#include "blinkit/blink/renderer/core/dom/node_traversal.h"
#include "blinkit/blink/renderer/core/dom/shadow/ComposedTreeTraversal.h"

namespace blink {

template <typename Strategy>
class PositionTemplate;

template <typename Strategy>
class PositionIteratorAlgorithm;

// Editing algorithm defined on node traversal.
template <typename Traversal>
class EditingAlgorithm : public Traversal
{
    STATIC_ONLY(EditingAlgorithm);
public:
    static int caretMaxOffset(const Node&);
    // TODO(yosin) We should make following functions to take |Node&| instead
    // of |Node*|.
    static bool isEmptyNonEditableNodeInEditable(const Node*);
    static bool editingIgnoresContent(const Node*);

    // This method is used to create positions in the DOM. It returns the
    // maximum valid offset in a node. It returns 1 for some elements even
    // though they do not have children, which creates technically invalid DOM
    // Positions. Be sure to call |parentAnchoredEquivalent()| on a Position
    // before using it to create a DOM Range, or an exception will be thrown.
    static int lastOffsetForEditing(const Node*);
    static Node* rootUserSelectAllForNode(Node*);
};

extern template class CORE_EXTERN_TEMPLATE_EXPORT EditingAlgorithm<NodeTraversal>;
extern template class CORE_EXTERN_TEMPLATE_EXPORT EditingAlgorithm<ComposedTreeTraversal>;

// DOM tree version of editing algorithm
using EditingStrategy = EditingAlgorithm<NodeTraversal>;
// Composed tree version of editing algorithm
using EditingInComposedTreeStrategy = EditingAlgorithm<ComposedTreeTraversal>;

} // namespace blink

#endif // EditingStrategy_h
