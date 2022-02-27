#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: deferred_legacy_style_interpolation.h
// Description: DeferredLegacyStyleInterpolation Class
//      Author: Ziming Li
//     Created: 2022-02-27
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DeferredLegacyStyleInterpolation_h
#define DeferredLegacyStyleInterpolation_h

#include "blinkit/blink/renderer/core/animation/LegacyStyleInterpolation.h"
#include "blinkit/blink/renderer/core/animation/StyleInterpolation.h"
#include "blinkit/blink/renderer/core/css/CSSValue.h"

namespace blink {

class CSSBasicShapeCircleValue;
class CSSBasicShapeEllipseValue;
class CSSBasicShapePolygonValue;
class CSSBasicShapeInsetValue;
class CSSImageValue;
class CSSPrimitiveValue;
class CSSQuadValue;
class CSSShadowValue;
class CSSSVGDocumentValue;
class CSSValueList;
class CSSValuePair;

class DeferredLegacyStyleInterpolation : public StyleInterpolation
{
public:
    static PassRefPtr<DeferredLegacyStyleInterpolation> create(CSSValue *start, CSSValue *end, CSSPropertyID id)
    {
        return adoptRef(new DeferredLegacyStyleInterpolation(start, end, id));
    }

    void apply(StyleResolverState&) const override;

    static bool interpolationRequiresStyleResolve(const CSSValue&);
    static bool interpolationRequiresStyleResolve(const CSSPrimitiveValue&);
    static bool interpolationRequiresStyleResolve(const CSSImageValue&);
    static bool interpolationRequiresStyleResolve(const CSSShadowValue&);
    static bool interpolationRequiresStyleResolve(const CSSSVGDocumentValue&);
    static bool interpolationRequiresStyleResolve(const CSSValueList&);
    static bool interpolationRequiresStyleResolve(const CSSValuePair&);
    static bool interpolationRequiresStyleResolve(const CSSBasicShapeCircleValue&);
    static bool interpolationRequiresStyleResolve(const CSSBasicShapeEllipseValue&);
    static bool interpolationRequiresStyleResolve(const CSSBasicShapePolygonValue&);
    static bool interpolationRequiresStyleResolve(const CSSBasicShapeInsetValue&);
    static bool interpolationRequiresStyleResolve(const CSSQuadValue&);

private:
    DeferredLegacyStyleInterpolation(CSSValue *start, CSSValue *end, CSSPropertyID id)
        : StyleInterpolation(InterpolableNumber::create(0), InterpolableNumber::create(1), id)
        , m_startCSSValue(start)
        , m_endCSSValue(end)
        , m_outdated(true)
    {
    }

    GCRefPtr<CSSValue> m_startCSSValue;
    GCRefPtr<CSSValue> m_endCSSValue;
    mutable RefPtr<LegacyStyleInterpolation> m_innerInterpolation;
    mutable bool m_outdated;
};

}

#endif
