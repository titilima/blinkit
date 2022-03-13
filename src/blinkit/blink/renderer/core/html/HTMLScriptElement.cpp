// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: HTMLScriptElement.cpp
// Description: HTMLScriptElement Class
//      Author: Ziming Li
//     Created: 2021-07-24
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2001 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2003, 2004, 2005, 2006, 2007, 2008 Apple Inc. All rights reserved.
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

#include "./HTMLScriptElement.h"

#include "blinkit/blink/renderer/bindings/core/exception_state_placeholder.h"
#include "blinkit/blink/renderer/core/dom/Text.h"

namespace blink {

using namespace HTMLNames;

inline HTMLScriptElement::HTMLScriptElement(Document& document, bool wasInsertedByParser, bool alreadyStarted)
    : ScriptElementImpl(document, wasInsertedByParser, alreadyStarted)
{
}

GCRefPtr<HTMLScriptElement> HTMLScriptElement::create(Document& document, bool wasInsertedByParser, bool alreadyStarted)
{
    return GCWrapShared(new HTMLScriptElement(document, wasInsertedByParser, alreadyStarted));
}

void HTMLScriptElement::setText(const String &value)
{
    setTextContent(value);
}

KURL HTMLScriptElement::src() const
{
    return document().completeURL(sourceAttributeValue());
}

GCRefPtr<Element> HTMLScriptElement::cloneElementWithoutAttributesAndChildren()
{
    return GCWrapShared(new HTMLScriptElement(document(), false, loader()->alreadyStarted()));
}

}
