// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: PaintPropertyFunctions.cpp
// Description: PaintPropertyFunctions Class
//      Author: Ziming Li
//     Created: 2019-05-24
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/animation/PaintPropertyFunctions.h"

#include "core/css/StyleColor.h"
#include "core/style/ComputedStyle.h"

namespace blink {

bool PaintPropertyFunctions::getInitialColor(CSSPropertyID property, StyleColor& result)
{
    return getColor(property, *ComputedStyle::initialStyle(), result);
}

static bool getColorFromPaint(const SVGPaintType type, const Color color, StyleColor& result)
{
    switch (type) {
    case SVG_PAINTTYPE_RGBCOLOR:
        result = color;
        return true;
    case SVG_PAINTTYPE_CURRENTCOLOR:
        result = StyleColor::currentColor();
        return true;
    default:
        return false;
    }
}

bool PaintPropertyFunctions::getColor(CSSPropertyID property, const ComputedStyle& style, StyleColor& result)
{
    ASSERT_NOT_REACHED();
    return false;
}

void PaintPropertyFunctions::setColor(CSSPropertyID property, ComputedStyle& style, const Color& color)
{
    ASSERT_NOT_REACHED();
}


} // namespace blink
