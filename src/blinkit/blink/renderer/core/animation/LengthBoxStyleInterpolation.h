// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: LengthBoxStyleInterpolation.h
// Description: LengthBoxStyleInterpolation Class
//      Author: Ziming Li
//     Created: 2021-10-19
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LengthBoxStyleInterpolation_h
#define LengthBoxStyleInterpolation_h

#include "core/CoreExport.h"
#include "core/animation/LengthStyleInterpolation.h"
#include "core/css/CSSBorderImageSliceValue.h"

namespace blink {

class CORE_EXPORT LengthBoxStyleInterpolation : public StyleInterpolation {
public:
    static PassRefPtr<LengthBoxStyleInterpolation> maybeCreateFrom(CSSValue&, CSSValue&, CSSPropertyID);

    static bool usesDefaultInterpolation(const CSSValue&, const CSSValue&);

    void apply(StyleResolverState&) const override;

private:
    LengthBoxStyleInterpolation(PassOwnPtr<InterpolableValue> startInterpolation, PassOwnPtr<InterpolableValue> endInterpolation, CSSPropertyID id, CSSValue* startCSS, CSSValue* endCSS)
        : StyleInterpolation(startInterpolation, endInterpolation, id)
        , m_startCSSValue(startCSS)
        , m_endCSSValue(endCSS)
    { }

    static PassOwnPtr<InterpolableValue> lengthBoxtoInterpolableValue(const CSSValue&, const CSSValue&, bool);
    static GCRefPtr<CSSValue> interpolableValueToLengthBox(InterpolableValue*, const CSSValue&, const CSSValue&);

    GCRefPtr<CSSValue> m_startCSSValue;
    GCRefPtr<CSSValue> m_endCSSValue;

    friend class AnimationLengthBoxStyleInterpolationTest;
};

} // namespace blink

#endif // LengthBoxStyleInterpolation_h
