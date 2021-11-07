// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: BaseButtonInputType.cpp
// Description: BaseButtonInputType Class
//      Author: Ziming Li
//     Created: 2021-11-06
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2010 Google Inc. All rights reserved.
 * Copyright (C) 2011 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "core/html/forms/BaseButtonInputType.h"

#include "core/HTMLNames.h"
#include "core/dom/Text.h"
#include "core/dom/shadow/ShadowRoot.h"
#include "core/html/HTMLInputElement.h"
#include "core/html/parser/HTMLParserIdioms.h"
#include "core/layout/LayoutButton.h"

namespace blink {

using namespace HTMLNames;

void BaseButtonInputType::createShadowSubtree()
{
    ASSERT(element().userAgentShadowRoot());
    GCRefPtr<Text> text = Text::create(element().document(), displayValue());
    element().userAgentShadowRoot()->appendChild(text.get());
}

void BaseButtonInputType::valueAttributeChanged()
{
    toText(element().userAgentShadowRoot()->firstChild())->setData(displayValue());
}

String BaseButtonInputType::displayValue() const
{
    return element().valueWithDefault().removeCharacters(isHTMLLineBreak);
}

bool BaseButtonInputType::shouldSaveAndRestoreFormControlState() const
{
    return false;
}

void BaseButtonInputType::appendToFormData(FormData&) const
{
}

LayoutObject* BaseButtonInputType::createLayoutObject(const ComputedStyle&) const
{
    return new LayoutButton(&element());
}

bool BaseButtonInputType::storesValueSeparateFromAttribute()
{
    return false;
}

void BaseButtonInputType::setValue(const String& sanitizedValue, bool, TextFieldEventBehavior)
{
    element().setAttribute(valueAttr, AtomicString(sanitizedValue));
}

} // namespace blink
