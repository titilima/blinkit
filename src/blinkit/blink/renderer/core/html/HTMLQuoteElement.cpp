// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: HTMLQuoteElement.cpp
// Description: HTMLQuoteElement Class
//      Author: Ziming Li
//     Created: 2021-07-30
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2000 Simon Hausmann <hausmann@kde.org>
 * Copyright (C) 2003, 2006, 2010 Apple Inc. All rights reserved.
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

#include "core/html/HTMLQuoteElement.h"

#include "core/HTMLNames.h"
#include "core/dom/StyleEngine.h"

namespace blink {

using namespace HTMLNames;

inline HTMLQuoteElement::HTMLQuoteElement(const QualifiedName& tagName, Document& document)
    : HTMLElement(tagName, document)
{
    ASSERT(hasTagName(qTag) || hasTagName(blockquoteTag));
}

DEFINE_ELEMENT_FACTORY_WITH_TAGNAME(HTMLQuoteElement)

bool HTMLQuoteElement::isURLAttribute(const Attribute& attribute) const
{
    ASSERT(false); // BKTODO: return attribute.name() == citeAttr || HTMLElement::isURLAttribute(attribute);
    return false;
}

bool HTMLQuoteElement::hasLegalLinkAttribute(const QualifiedName& name) const
{
    ASSERT(false); // BKTODO: return name == citeAttr || HTMLElement::hasLegalLinkAttribute(name);
    return false;
}

const QualifiedName& HTMLQuoteElement::subResourceAttributeName() const
{
    ASSERT(false); // BKTODO: return citeAttr;
    exit(0);
}

}
