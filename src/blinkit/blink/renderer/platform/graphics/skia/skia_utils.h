#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: skia_utils.h
// Description: Skia Utils
//      Author: Ziming Li
//     Created: 2022-02-23
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (c) 2006,2007,2008, Google Inc. All rights reserved.
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

// All of the functions in this file should move to new homes and this file should be deleted.

#ifndef SkiaUtils_h
#define SkiaUtils_h

#include "blinkit/blink/renderer/platform/graphics/GraphicsTypes.h"
#include "blinkit/blink/renderer/platform/graphics/Image.h"
#include "blinkit/blink/renderer/platform/transforms/AffineTransform.h"
#include "blinkit/blink/renderer/wtf/MathExtras.h"
#include "third_party/skia/include/core/SkCanvas.h"
#include "third_party/zed/include/zed/file/path.hpp"

namespace blink {

class GraphicsContext;

SkXfermode::Mode WebCoreCompositeToSkiaComposite(CompositeOperator, WebBlendMode = WebBlendModeNormal);
CompositeOperator compositeOperatorFromSkia(SkXfermode::Mode);
WebBlendMode blendModeFromSkia(SkXfermode::Mode);

// Map alpha values from [0, 1] to [0, 256] for alpha blending.
int clampedAlphaForBlending(float);

// Multiply a color's alpha channel by an additional alpha factor where
// alpha is in the range [0, 1].
SkColor scaleAlpha(SkColor, float);

// Multiply a color's alpha channel by an additional alpha factor where
// alpha is in the range [0, 256].
SkColor scaleAlpha(SkColor, int);

// Skia has problems when passed infinite, etc floats, filter them to 0.
inline SkScalar WebCoreFloatToSkScalar(float f)
{
    return SkFloatToScalar(std::isfinite(f) ? f : 0);
}

inline SkScalar WebCoreDoubleToSkScalar(double d)
{
    return SkDoubleToScalar(std::isfinite(d) ? d : 0);
}

inline bool WebCoreFloatNearlyEqual(float a, float b)
{
    return SkScalarNearlyEqual(WebCoreFloatToSkScalar(a), WebCoreFloatToSkScalar(b));
}

inline SkPath::FillType WebCoreWindRuleToSkFillType(WindRule rule)
{
    return static_cast<SkPath::FillType>(rule);
}

inline WindRule SkFillTypeToWindRule(SkPath::FillType fillType)
{
    switch (fillType) {
    case SkPath::kWinding_FillType:
    case SkPath::kEvenOdd_FillType:
        return static_cast<WindRule>(fillType);
    default:
        ASSERT_NOT_REACHED();
        break;
    }
    return RULE_NONZERO;
}

SkMatrix affineTransformToSkMatrix(const AffineTransform&);

bool nearlyIntegral(float value);

InterpolationQuality limitInterpolationQuality(const GraphicsContext&, InterpolationQuality resampling);

InterpolationQuality computeInterpolationQuality(
    float srcWidth,
    float srcHeight,
    float destWidth,
    float destHeight,
    bool isDataComplete = true);

// This replicates the old skia behavior when it used to take radius for blur. Now it takes sigma.
inline SkScalar skBlurRadiusToSigma(SkScalar radius)
{
    SkASSERT(radius >= 0);
    if (radius == 0)
        return 0.0f;
    return 0.288675f * radius + 0.5f;
}

template<typename PrimitiveType>
void drawPlatformFocusRing(const PrimitiveType&, SkCanvas*, SkColor, int width);

// TODO(fmalita): remove in favor of direct SrcRectConstraint use.
inline SkCanvas::SrcRectConstraint WebCoreClampingModeToSkiaRectConstraint(Image::ImageClampingMode clampMode)
{
    return clampMode == Image::ClampImageToSourceRect
        ? SkCanvas::kStrict_SrcRectConstraint
        : SkCanvas::kFast_SrcRectConstraint;
}

} // namespace blink

namespace BlinKit {
bool SaveBitmapAsPNG(const SkBitmap &bitmap, zed::path::psz_t filePath);
}

#endif  // SkiaUtils_h
