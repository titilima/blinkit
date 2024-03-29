// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: HTMLMenuItemElement.cpp
// Description: HTMLMenuItemElement Class
//      Author: Ziming Li
//     Created: 2022-03-02
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "./HTMLMenuItemElement.h"

#include "blinkit/blink/renderer/core/HTMLNames.h"
#include "blinkit/blink/renderer/core/dom/element_traversal.h"
#include "blinkit/blink/renderer/core/events/Event.h"
#include "blinkit/blink/renderer/core/frame/UseCounter.h"

namespace blink {

using namespace HTMLNames;

inline HTMLMenuItemElement::HTMLMenuItemElement(Document& document)
    : HTMLElement(HTMLNames::menuitemTag, document)
{
    UseCounter::count(document, UseCounter::MenuItemElement);
}

void HTMLMenuItemElement::defaultEventHandler(Event* event)
{
    if (event->type() == EventTypeNames::click) {
        if (equalIgnoringCase(fastGetAttribute(typeAttr), "checkbox")) {
            if (fastHasAttribute(checkedAttr))
                removeAttribute(checkedAttr);
            else
                setAttribute(checkedAttr, "checked");
        } else if (equalIgnoringCase(fastGetAttribute(typeAttr), "radio")) {
            if (Element* parent = parentElement()) {
                const AtomicString& group = fastGetAttribute(radiogroupAttr);
                for (HTMLMenuItemElement& menuItem : Traversal<HTMLMenuItemElement>::childrenOf(*parent)) {
                    if (!menuItem.fastHasAttribute(checkedAttr))
                        continue;
                    const AtomicString& groupAttr = menuItem.fastGetAttribute(radiogroupAttr);
                    if (equalIgnoringNullity(groupAttr.impl(), group.impl()))
                        menuItem.removeAttribute(checkedAttr);
                }
            }
            setAttribute(checkedAttr, "checked");
        }
        event->setDefaultHandled();
    }
}

DEFINE_NODE_FACTORY(HTMLMenuItemElement)

} // namespace blink
