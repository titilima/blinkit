// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: HTMLPictureElement.cpp
// Description: HTMLPictureElement Class
//      Author: Ziming Li
//     Created: 2022-03-02
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "./HTMLPictureElement.h"

#include "core/HTMLNames.h"
#include "blinkit/blink/renderer/core/dom/element_traversal.h"
#include "blinkit/blink/renderer/core/frame/UseCounter.h"
#include "blinkit/blink/renderer/core/html/HTMLImageElement.h"
#include "blinkit/blink/renderer/core/loader/ImageLoader.h"

namespace blink {

using namespace HTMLNames;

inline HTMLPictureElement::HTMLPictureElement(Document& document)
    : HTMLElement(pictureTag, document)
{
}

DEFINE_NODE_FACTORY(HTMLPictureElement)

void HTMLPictureElement::sourceOrMediaChanged()
{
    for (HTMLImageElement* imageElement = Traversal<HTMLImageElement>::firstChild(*this); imageElement; imageElement = Traversal<HTMLImageElement>::nextSibling(*imageElement)) {
        imageElement->selectSourceURL(ImageLoader::UpdateNormal);
    }
}

Node::InsertionNotificationRequest HTMLPictureElement::insertedInto(ContainerNode* insertionPoint)
{
    UseCounter::count(document(), UseCounter::Picture);
    return HTMLElement::insertedInto(insertionPoint);
}

} // namespace
