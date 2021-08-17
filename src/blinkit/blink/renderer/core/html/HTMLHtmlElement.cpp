// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: HTMLHtmlElement.cpp
// Description: HTMLHtmlElement Class
//      Author: Ziming Li
//     Created: 2021-07-24
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2000 Simon Hausmann (hausmann@kde.org)
 *           (C) 2001 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2004, 2006, 2010 Apple Inc. All rights reserved.
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

#include "core/html/HTMLHtmlElement.h"

#include "core/HTMLNames.h"
#include "core/dom/Document.h"
#include "core/dom/DocumentParser.h"
#include "core/frame/LocalFrame.h"
#include "core/loader/DocumentLoader.h"
#include "core/loader/FrameLoader.h"
// BKTODO: #include "core/loader/appcache/ApplicationCacheHost.h"

namespace blink {

using namespace HTMLNames;

inline HTMLHtmlElement::HTMLHtmlElement(Document& document)
    : HTMLElement(htmlTag, document)
{
}

DEFINE_NODE_FACTORY(HTMLHtmlElement)

bool HTMLHtmlElement::isURLAttribute(const Attribute& attribute) const
{
    ASSERT(false); // BKTODO: return attribute.name() == manifestAttr || HTMLElement::isURLAttribute(attribute);
    return HTMLElement::isURLAttribute(attribute);
}

void HTMLHtmlElement::insertedByParser()
{
#if 0 // BKTODO:
    // When parsing a fragment, its dummy document has a null parser.
    if (!document().parser() || !document().parser()->documentWasLoadedAsPartOfNavigation())
        return;

    if (!document().frame())
        return;

    DocumentLoader* documentLoader = document().frame()->loader().documentLoader();
    if (!documentLoader)
        return;

    const AtomicString& manifest = fastGetAttribute(manifestAttr);
    if (manifest.isEmpty())
        documentLoader->applicationCacheHost()->selectCacheWithoutManifest();
    else
        documentLoader->applicationCacheHost()->selectCacheWithManifest(document().completeURL(manifest));
#endif
}

}
