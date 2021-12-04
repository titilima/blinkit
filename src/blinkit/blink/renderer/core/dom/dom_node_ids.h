#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: dom_node_ids.h
// Description: DOMNodeIds Class
//      Author: Ziming Li
//     Created: 2021-07-12
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DOMNodeIds_h
#define DOMNodeIds_h

#include "blinkit/blink/renderer/core/dom/Node.h"
#include "blinkit/blink/renderer/wtf/Allocator.h"

namespace blink {

class DOMNodeIds
{
    STATIC_ONLY(DOMNodeIds);
public:
    static int idForNode(Node *node);
    static Node* nodeForId(int id);
};

} // namespace blink


#endif // DOMNodeIds_h
