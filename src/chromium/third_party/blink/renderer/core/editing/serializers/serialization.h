// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: serialization.h
// Description: Serialization Helpers
//      Author: Ziming Li
//     Created: 2020-03-08
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2004 Apple Computer, Inc.  All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef BLINKIT_BLINK_SERIALIZATION_H
#define BLINKIT_BLINK_SERIALIZATION_H

#pragma once

#include "third_party/blink/renderer/core/dom/parser_content_policy.h"
#include "third_party/blink/renderer/platform/wtf/text/wtf_string.h"

namespace blink {

class ContainerNode;
class Document;
class DocumentFragment;
class Element;
class ExceptionState;
class Node;
class Text;

enum EChildrenOnly { kIncludeNode, kChildrenOnly };
enum EAbsoluteURLs { kDoNotResolveURLs, kResolveAllURLs, kResolveNonLocalURLs };

DocumentFragment* CreateFragmentForInnerOuterHTML(
    const String &markup,
    Element *contextElement,
    ParserContentPolicy parserContentPolicy,
    const char *method,
    ExceptionState &exceptionState
);

// These methods are used by HTMLElement & ShadowRoot to replace the
// children with respected fragment/text.
void ReplaceChildrenWithFragment(ContainerNode *container, DocumentFragment *fragment, std::vector<Node *> &detachedChildren, ExceptionState &exceptionState);

String CreateMarkup(const Node *node, EChildrenOnly childrenOnly = kIncludeNode, EAbsoluteURLs shouldResolveUrls = kDoNotResolveURLs);

void MergeWithNextTextNode(Text *textNode, std::vector<Node *> &detachedNodes, ExceptionState &exceptionState);

} // namespace blink

#endif // BLINKIT_BLINK_SERIALIZATION_H
