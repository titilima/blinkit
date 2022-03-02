#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: NonDocumentTypeChildNode.h
// Description: NonDocumentTypeChildNode Class
//      Author: Ziming Li
//     Created: 2022-03-02
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NonDocumentTypeChildNode_h
#define NonDocumentTypeChildNode_h

#include "blinkit/blink/renderer/core/dom/element_traversal.h"

namespace blink {

class NonDocumentTypeChildNode {
public:
    static Element* previousElementSibling(Node& node)
    {
        return ElementTraversal::previousSibling(node);
    }

    static Element* nextElementSibling(Node& node)
    {
        return ElementTraversal::nextSibling(node);
    }

    static HTMLSlotElement* assignedSlotForBinding(Node& node)
    {
        return node.assignedSlotForBinding();
    }
};

} // namespace blink

#endif // NonDocumentTypeChildNode_h
