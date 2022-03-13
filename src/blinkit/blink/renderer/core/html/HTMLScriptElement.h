// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: HTMLScriptElement.h
// Description: HTMLScriptElement Class
//      Author: Ziming Li
//     Created: 2021-11-06
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 * Copyright (C) 2003, 2004, 2005, 2006, 2007, 2008 Apple Inc. All rights reserved.
 * Copyright (C) 2008 Nikolas Zimmermann <zimmermann@kde.org>
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
 *
 */

#ifndef HTMLScriptElement_h
#define HTMLScriptElement_h

#include "blinkit/blink/renderer/core/dom/script_element_impl.h"
#include "blinkit/blink/renderer/core/html/HTMLElement.h"

namespace blink {

class HTMLScriptElement final : public BlinKit::ScriptElementImpl<HTMLElement>
{
public:
    static GCRefPtr<HTMLScriptElement> create(Document&, bool wasInsertedByParser, bool alreadyStarted = false);

    String text() { return textFromChildren(); }
    void setText(const String&);

    KURL src() const;

    constexpr bool async(void) const { return false; }
private:
    HTMLScriptElement(Document&, bool wasInsertedByParser, bool alreadyStarted);

    GCRefPtr<Element> cloneElementWithoutAttributesAndChildren() override;
};

} // namespace blink

#endif // HTMLScriptElement_h
