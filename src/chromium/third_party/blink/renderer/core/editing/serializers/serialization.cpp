// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: serialization.cpp
// Description: Serialization Helpers
//      Author: Ziming Li
//     Created: 2020-03-08
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009 Apple Inc. All rights
 * reserved.
 * Copyright (C) 2008, 2009, 2010, 2011 Google Inc. All rights reserved.
 * Copyright (C) 2011 Igalia S.L.
 * Copyright (C) 2011 Motorola Mobility. All rights reserved.
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

#include "serialization.h"

#include "third_party/blink/renderer/core/dom/child_list_mutation_scope.h"
#include "third_party/blink/renderer/core/dom/document_fragment.h"
#include "third_party/blink/renderer/core/dom/element.h"
#include "third_party/blink/renderer/core/editing/serializers/markup_accumulator.h"

using namespace BlinKit;

namespace blink {

DocumentFragment* CreateFragmentForInnerOuterHTML(
    const String &markup,
    Element *contextElement,
    ParserContentPolicy parserContentPolicy,
    const char *method,
    ExceptionState &exceptionState)
{
    ASSERT(nullptr != contextElement);
#ifdef BLINKIT_CRAWLER_ONLY
    Document &document = contextElement->GetDocument();
#else
    Document& document =
        IsHTMLTemplateElement(*context_element)
        ? context_element->GetDocument().EnsureTemplateDocument()
        : context_element->GetDocument();
#endif

    DocumentFragment *fragment = DocumentFragment::Create(document);
    fragment->ParseHTML(markup, contextElement, parserContentPolicy);
    return fragment;
}

String CreateMarkup(const Node *node, EChildrenOnly childrenOnly, EAbsoluteURLs shouldResolveUrls)
{
    if (nullptr == node)
        return "";

    MarkupAccumulator accumulator(shouldResolveUrls);
    return SerializeNodes<EditingStrategy>(accumulator, const_cast<Node &>(*node), childrenOnly);
}

void ReplaceChildrenWithFragment(ContainerNode *container, DocumentFragment *fragment, ExceptionState &exceptionState)
{
    ASSERT(nullptr != container);
    ContainerNode *containerNode = container;

    ChildListMutationScope mutation(*containerNode);

    if (nullptr == fragment->firstChild())
    {
        containerNode->RemoveChildren();
        return;
    }

    // FIXME: No need to replace the child it is a text node and its contents are
    // already == text.
    if (containerNode->HasOneChild())
    {
        containerNode->ReplaceChild(fragment, containerNode->firstChild(), exceptionState);
        return;
    }

    containerNode->RemoveChildren();
    containerNode->appendChild(fragment, exceptionState);
}

} // namespace blink
