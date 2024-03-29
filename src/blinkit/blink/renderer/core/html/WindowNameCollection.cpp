// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: WindowNameCollection.cpp
// Description: WindowNameCollection Class
//      Author: Ziming Li
//     Created: 2021-07-25
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/html/WindowNameCollection.h"

#include "core/html/HTMLImageElement.h"

namespace blink {

WindowNameCollection::WindowNameCollection(ContainerNode& document, const AtomicString& name)
    : HTMLNameCollection(document, WindowNamedItems, name)
{
}

bool WindowNameCollection::elementMatches(const Element& element) const
{
    ASSERT(false); // BKTODO:
#if 0
    // Match only images, forms, embeds and objects by name,
    // but anything by id
    if (isHTMLImageElement(element)
        || isHTMLFormElement(element)
        || isHTMLEmbedElement(element)
        || isHTMLObjectElement(element)) {
        if (element.getNameAttribute() == m_name)
            return true;
    }
#endif
    return element.getIdAttribute() == m_name;
}

} // namespace blink
