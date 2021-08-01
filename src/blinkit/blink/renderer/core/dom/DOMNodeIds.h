// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: DOMNodeIds.h
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

#include "core/CoreExport.h"
#include "core/dom/Node.h"
#include "wtf/Allocator.h"

namespace blink {

class CORE_EXPORT DOMNodeIds {
    STATIC_ONLY(DOMNodeIds);
public:
    static int idForNode(Node*);
    static Node* nodeForId(int id);
};

} // namespace blink


#endif // DOMNodeIds_h
