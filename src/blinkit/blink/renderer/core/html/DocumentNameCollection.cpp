// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: DocumentNameCollection.cpp
// Description: DocumentNameCollection Class
//      Author: Ziming Li
//     Created: 2021-07-24
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/html/DocumentNameCollection.h"

#if 0 // BKTODO:
#include "core/html/HTMLEmbedElement.h"
#include "core/html/HTMLObjectElement.h"
#endif

namespace blink {

DocumentNameCollection::DocumentNameCollection(ContainerNode& document, const AtomicString& name)
    : HTMLNameCollection(document, DocumentNamedItems, name)
{
}

bool DocumentNameCollection::elementMatches(const HTMLElement& element) const
{
    ASSERT(false); // BKTODO:
#if 0
    // Match images, forms, embeds, objects and iframes by name,
    // object by id, and images by id but only if they have
    // a name attribute (this very strange rule matches IE)
    if (isHTMLFormElement(element) || isHTMLIFrameElement(element) || (isHTMLEmbedElement(element) && toHTMLEmbedElement(element).isExposed()))
        return element.getNameAttribute() == m_name;
    if (isHTMLObjectElement(element) && toHTMLObjectElement(element).isExposed())
        return element.getNameAttribute() == m_name || element.getIdAttribute() == m_name;
    if (isHTMLImageElement(element))
        return element.getNameAttribute() == m_name || (element.getIdAttribute() == m_name && element.hasName());
#endif
    return false;
}

} // namespace blink
