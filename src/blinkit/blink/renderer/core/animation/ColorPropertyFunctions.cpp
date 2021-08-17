// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: ColorPropertyFunctions.cpp
// Description: ColorPropertyFunctions Class
//      Author: Ziming Li
//     Created: 2021-08-13
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/animation/ColorPropertyFunctions.h"

#include "core/style/ComputedStyle.h"

namespace blink {

StyleColor ColorPropertyFunctions::getInitialColor(CSSPropertyID property)
{
    return getUnvisitedColor(property, *ComputedStyle::initialStyle());
}

StyleColor ColorPropertyFunctions::getUnvisitedColor(CSSPropertyID property, const ComputedStyle& style)
{
    switch (property) {
    case CSSPropertyBackgroundColor:
        return style.backgroundColor();
    case CSSPropertyBorderLeftColor:
        return style.borderLeftColor();
    case CSSPropertyBorderRightColor:
        return style.borderRightColor();
    case CSSPropertyBorderTopColor:
        return style.borderTopColor();
    case CSSPropertyBorderBottomColor:
        return style.borderBottomColor();
    case CSSPropertyColor:
        return style.color();
    case CSSPropertyOutlineColor:
        return style.outlineColor();
    case CSSPropertyWebkitColumnRuleColor:
        return style.columnRuleColor();
    case CSSPropertyWebkitTextEmphasisColor:
        return style.textEmphasisColor();
    case CSSPropertyWebkitTextFillColor:
        return style.textFillColor();
    case CSSPropertyWebkitTextStrokeColor:
        return style.textStrokeColor();
#if 0 // BKTODO:
    case CSSPropertyFloodColor:
        return style.floodColor();
    case CSSPropertyLightingColor:
        return style.lightingColor();
    case CSSPropertyStopColor:
        return style.stopColor();
#endif
    case CSSPropertyWebkitTapHighlightColor:
        return style.tapHighlightColor();
    case CSSPropertyTextDecorationColor:
        return style.textDecorationColor();
    default:
        ASSERT_NOT_REACHED();
        return StyleColor::currentColor();
    }
}

StyleColor ColorPropertyFunctions::getVisitedColor(CSSPropertyID property, const ComputedStyle& style)
{
    switch (property) {
    case CSSPropertyBackgroundColor:
        return style.visitedLinkBackgroundColor();
    case CSSPropertyBorderLeftColor:
        return style.visitedLinkBorderLeftColor();
    case CSSPropertyBorderRightColor:
        return style.visitedLinkBorderRightColor();
    case CSSPropertyBorderTopColor:
        return style.visitedLinkBorderTopColor();
    case CSSPropertyBorderBottomColor:
        return style.visitedLinkBorderBottomColor();
    case CSSPropertyColor:
        return style.visitedLinkColor();
    case CSSPropertyOutlineColor:
        return style.visitedLinkOutlineColor();
    case CSSPropertyWebkitColumnRuleColor:
        return style.visitedLinkColumnRuleColor();
    case CSSPropertyWebkitTextEmphasisColor:
        return style.visitedLinkTextEmphasisColor();
    case CSSPropertyWebkitTextFillColor:
        return style.visitedLinkTextFillColor();
    case CSSPropertyWebkitTextStrokeColor:
        return style.visitedLinkTextStrokeColor();
#if 0 // BKTODO:
    case CSSPropertyFloodColor:
        return style.floodColor();
    case CSSPropertyLightingColor:
        return style.lightingColor();
    case CSSPropertyStopColor:
        return style.stopColor();
#endif
    case CSSPropertyWebkitTapHighlightColor:
        return style.tapHighlightColor();
    case CSSPropertyTextDecorationColor:
        return style.visitedLinkTextDecorationColor();
    default:
        ASSERT_NOT_REACHED();
        return StyleColor::currentColor();
    }
}

void ColorPropertyFunctions::setUnvisitedColor(CSSPropertyID property, ComputedStyle& style, const Color& color)
{
    switch (property) {
    case CSSPropertyBackgroundColor:
        style.setBackgroundColor(color);
        return;
    case CSSPropertyBorderBottomColor:
        style.setBorderBottomColor(color);
        return;
    case CSSPropertyBorderLeftColor:
        style.setBorderLeftColor(color);
        return;
    case CSSPropertyBorderRightColor:
        style.setBorderRightColor(color);
        return;
    case CSSPropertyBorderTopColor:
        style.setBorderTopColor(color);
        return;
    case CSSPropertyColor:
        style.setColor(color);
        return;
#if 0 // BKTODO:
    case CSSPropertyFloodColor:
        style.setFloodColor(color);
        return;
    case CSSPropertyLightingColor:
        style.setLightingColor(color);
        return;
#endif
    case CSSPropertyOutlineColor:
        style.setOutlineColor(color);
        return;
#if 0 // BKTODO:
    case CSSPropertyStopColor:
        style.setStopColor(color);
        return;
#endif
    case CSSPropertyTextDecorationColor:
        style.setTextDecorationColor(color);
        return;
    case CSSPropertyWebkitColumnRuleColor:
        style.setColumnRuleColor(color);
        return;
    case CSSPropertyWebkitTextStrokeColor:
        style.setTextStrokeColor(color);
        return;
    default:
        ASSERT_NOT_REACHED();
        return;
    }
}

void ColorPropertyFunctions::setVisitedColor(CSSPropertyID property, ComputedStyle& style, const Color& color)
{
    switch (property) {
    case CSSPropertyBackgroundColor:
        style.setVisitedLinkBackgroundColor(color);
        return;
    case CSSPropertyBorderBottomColor:
        style.setVisitedLinkBorderBottomColor(color);
        return;
    case CSSPropertyBorderLeftColor:
        style.setVisitedLinkBorderLeftColor(color);
        return;
    case CSSPropertyBorderRightColor:
        style.setVisitedLinkBorderRightColor(color);
        return;
    case CSSPropertyBorderTopColor:
        style.setVisitedLinkBorderTopColor(color);
        return;
    case CSSPropertyColor:
        style.setVisitedLinkColor(color);
        return;
#if 0 // BKTODO:
    case CSSPropertyFloodColor:
        style.setFloodColor(color);
        return;
    case CSSPropertyLightingColor:
        style.setLightingColor(color);
        return;
#endif
    case CSSPropertyOutlineColor:
        style.setVisitedLinkOutlineColor(color);
        return;
#if 0 // BKTODO:
    case CSSPropertyStopColor:
        style.setStopColor(color);
        return;
#endif
    case CSSPropertyTextDecorationColor:
        style.setVisitedLinkTextDecorationColor(color);
        return;
    case CSSPropertyWebkitColumnRuleColor:
        style.setVisitedLinkColumnRuleColor(color);
        return;
    case CSSPropertyWebkitTextStrokeColor:
        style.setVisitedLinkTextStrokeColor(color);
        return;
    default:
        ASSERT_NOT_REACHED();
        return;
    }
}

} // namespace blink
