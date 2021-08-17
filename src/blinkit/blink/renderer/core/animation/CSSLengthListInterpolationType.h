// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: CSSLengthListInterpolationType.h
// Description: CSSLengthListInterpolationType Class
//      Author: Ziming Li
//     Created: 2021-08-13
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CSSLengthListInterpolationType_h
#define CSSLengthListInterpolationType_h

#include "core/animation/CSSInterpolationType.h"
#include "core/animation/CSSLengthInterpolationType.h"
#include "platform/Length.h"
#include "wtf/RefVector.h"

namespace blink {

class CSSLengthListInterpolationType : public CSSInterpolationType {
public:
    CSSLengthListInterpolationType(CSSPropertyID);

    PassOwnPtr<InterpolationValue> maybeConvertUnderlyingValue(const InterpolationEnvironment&) const final;
    void composite(UnderlyingValue&, double underlyingFraction, const InterpolationValue&) const final;
    void apply(const InterpolableValue&, const NonInterpolableValue*, InterpolationEnvironment&) const final;

private:
    PassOwnPtr<InterpolationValue> maybeConvertLengthList(const RefVector<Length>*, float zoom) const;

    PassOwnPtr<InterpolationValue> maybeConvertNeutral(const UnderlyingValue&, ConversionCheckers&) const final;
    PassOwnPtr<InterpolationValue> maybeConvertInitial() const final;
    PassOwnPtr<InterpolationValue> maybeConvertInherit(const StyleResolverState&, ConversionCheckers&) const final;
    PassOwnPtr<InterpolationValue> maybeConvertValue(const CSSValue&, const StyleResolverState&, ConversionCheckers&) const final;

    PassOwnPtr<PairwisePrimitiveInterpolation> mergeSingleConversions(InterpolationValue& startValue, InterpolationValue& endValue) const final;

    // BKTODO: const ValueRange m_valueRange;
};

} // namespace blink

#endif // CSSLengthListInterpolationType_h
