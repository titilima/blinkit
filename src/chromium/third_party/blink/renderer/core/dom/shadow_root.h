// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: shadow_root.h
// Description: ShadowRoot Class
//      Author: Ziming Li
//     Created: 2020-03-30
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2011 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef BLINKIT_BLINK_SHADOW_ROOT_H
#define BLINKIT_BLINK_SHADOW_ROOT_H

#pragma once

#include "third_party/blink/renderer/core/dom/document_fragment.h"
#include "third_party/blink/renderer/core/dom/element.h"
#include "third_party/blink/renderer/core/dom/tree_scope.h"

namespace blink {

class ShadowRoot final : public DocumentFragment, public TreeScope
{
public:
    Element& host(void) const
    {
        DCHECK(ParentOrShadowHostNode());
        return *ToElement(ParentOrShadowHostNode());
    }
};

DEFINE_NODE_TYPE_CASTS(ShadowRoot, IsShadowRoot());
DEFINE_TYPE_CASTS(ShadowRoot,
    TreeScope,
    treeScope,
    treeScope->RootNode().IsShadowRoot(),
    treeScope.RootNode().IsShadowRoot());
DEFINE_TYPE_CASTS(TreeScope, ShadowRoot, shadowRoot, true, true);

} // namespace blink

#endif // BLINKIT_BLINK_SHADOW_ROOT_H
