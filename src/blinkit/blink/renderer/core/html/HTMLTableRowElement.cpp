// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: HTMLTableRowElement.cpp
// Description: HTMLTableRowElement Class
//      Author: Ziming Li
//     Created: 2022-03-02
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1997 Martin Jones (mjones@kde.org)
 *           (C) 1997 Torben Weis (weis@kde.org)
 *           (C) 1998 Waldo Bastian (bastian@kde.org)
 *           (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 * Copyright (C) 2003, 2004, 2005, 2006, 2007, 2010 Apple Inc. All rights reserved.
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

#include "./HTMLTableRowElement.h"

#include "blinkit/blink/renderer/bindings/core/exception_state.h"
#include "blinkit/blink/renderer/core/HTMLNames.h"
#include "blinkit/blink/renderer/core/dom/element_traversal.h"
#include "blinkit/blink/renderer/core/dom/ExceptionCode.h"
#include "blinkit/blink/renderer/core/dom/NodeListsNodeData.h"
#include "blinkit/blink/renderer/core/html/HTMLCollection.h"
#include "blinkit/blink/renderer/core/html/HTMLTableCellElement.h"
#include "blinkit/blink/renderer/core/html/HTMLTableElement.h"
#include "blinkit/blink/renderer/core/html/HTMLTableRowsCollection.h"
#include "blinkit/blink/renderer/core/html/HTMLTableSectionElement.h"

namespace blink {

using namespace HTMLNames;

inline HTMLTableRowElement::HTMLTableRowElement(Document& document)
    : HTMLTablePartElement(trTag, document)
{
}

DEFINE_NODE_FACTORY(HTMLTableRowElement)

bool HTMLTableRowElement::hasLegalLinkAttribute(const QualifiedName& name) const
{
    return name == backgroundAttr || HTMLTablePartElement::hasLegalLinkAttribute(name);
}

const QualifiedName& HTMLTableRowElement::subResourceAttributeName() const
{
    return backgroundAttr;
}

int HTMLTableRowElement::rowIndex() const
{
    ContainerNode* maybeTable = parentNode();
    if (maybeTable && isHTMLTableSectionElement(maybeTable)) {
        // Skip THEAD, TBODY and TFOOT.
        maybeTable = maybeTable->parentNode();
    }
    if (!(maybeTable && isHTMLTableElement(maybeTable)))
        return -1;

    RefPtrWillBeRawPtr<HTMLTableRowsCollection> rows =
        toHTMLTableElement(maybeTable)->rows();
    HTMLTableRowElement* candidate = rows->item(0);
    for (int i = 0; candidate; i++, candidate = rows->item(i)) {
        if (this == candidate)
            return i;
    }

    return -1;
}

int HTMLTableRowElement::sectionRowIndex() const
{
    int rIndex = 0;
    const Node* n = this;
    do {
        n = n->previousSibling();
        if (n && isHTMLTableRowElement(*n))
            ++rIndex;
    } while (n);

    return rIndex;
}

PassRefPtrWillBeRawPtr<HTMLElement> HTMLTableRowElement::insertCell(int index, ExceptionState& exceptionState)
{
    RefPtrWillBeRawPtr<HTMLCollection> children = cells();
    int numCells = children ? children->length() : 0;
    if (index < -1 || index > numCells) {
        exceptionState.throwDOMException(IndexSizeError, "The value provided (" + String::number(index) + ") is outside the range [-1, " + String::number(numCells) + "].");
        return nullptr;
    }

    RefPtrWillBeRawPtr<HTMLTableCellElement> cell = HTMLTableCellElement::create(tdTag, document());
    if (numCells == index || index == -1)
        appendChild(cell, exceptionState);
    else
        insertBefore(cell, children->item(index), exceptionState);
    return cell.release();
}

void HTMLTableRowElement::deleteCell(int index, ExceptionState& exceptionState)
{
    RefPtrWillBeRawPtr<HTMLCollection> children = cells();
    int numCells = children ? children->length() : 0;
    if (index == -1)
        index = numCells-1;
    if (index >= 0 && index < numCells) {
        RefPtrWillBeRawPtr<Element> cell = children->item(index);
        HTMLElement::removeChild(cell.get(), exceptionState);
    } else {
        exceptionState.throwDOMException(IndexSizeError, "The value provided (" + String::number(index) + ") is outside the range [0, " + String::number(numCells) + ").");
    }
}

PassRefPtrWillBeRawPtr<HTMLCollection> HTMLTableRowElement::cells()
{
    return ensureCachedCollection<HTMLCollection>(TRCells);
}

}
