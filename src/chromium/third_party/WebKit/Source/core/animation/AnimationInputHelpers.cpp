// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: AnimationInputHelpers.cpp
// Description: AnimationInputHelpers Class
//      Author: Ziming Li
//     Created: 2019-03-31
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/animation/AnimationInputHelpers.h"

#include "core/HTMLNames.h"
#include "core/css/CSSValueList.h"
#include "core/css/parser/CSSParser.h"
#include "core/css/resolver/CSSToStyleMap.h"
#include "wtf/text/StringBuilder.h"

namespace blink {

CSSPropertyID AnimationInputHelpers::keyframeAttributeToCSSProperty(const String& property)
{
    // Disallow prefixed properties.
    if (property[0] == '-' || isASCIIUpper(property[0]))
        return CSSPropertyInvalid;
    if (property == "cssFloat")
        return CSSPropertyFloat;
    StringBuilder builder;
    for (size_t i = 0; i < property.length(); ++i) {
        if (isASCIIUpper(property[i]))
            builder.append('-');
        builder.append(property[i]);
    }
    return cssPropertyID(builder.toString());
}

CSSPropertyID AnimationInputHelpers::keyframeAttributeToPresentationAttribute(const String& property, const Element& element)
{
    return CSSPropertyInvalid;
}

using AttributeNameMap = HashMap<QualifiedName, const QualifiedName*>;

const AttributeNameMap& getSupportedAttributes()
{
    DEFINE_STATIC_LOCAL(AttributeNameMap, supportedAttributes, ());
    if (supportedAttributes.isEmpty()) {
        // Fill the set for the first use.
        // Animatable attributes from http://www.w3.org/TR/SVG/attindex.html
        const QualifiedName* attributes[] = {
            &HTMLNames::classAttr,
        };
        for (size_t i = 0; i < WTF_ARRAY_LENGTH(attributes); i++)
            supportedAttributes.set(*attributes[i], attributes[i]);
    }
    return supportedAttributes;
}

const QualifiedName* AnimationInputHelpers::keyframeAttributeToSVGAttribute(const String& property, Element& element)
{
    return nullptr;
}

PassRefPtr<TimingFunction> AnimationInputHelpers::parseTimingFunction(const String& string)
{
    if (string.isEmpty())
        return nullptr;

    RefPtrWillBeRawPtr<CSSValue> value = CSSParser::parseSingleValue(CSSPropertyTransitionTimingFunction, string);
    if (!value || !value->isValueList()) {
        ASSERT(!value || value->isCSSWideKeyword());
        return nullptr;
    }
    CSSValueList* valueList = toCSSValueList(value.get());
    if (valueList->length() > 1)
        return nullptr;
    return CSSToStyleMap::mapAnimationTimingFunction(*valueList->item(0), true);
}

} // namespace blink
