// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: LengthPairStyleInterpolation.cpp
// Description: LengthPairStyleInterpolation Class
//      Author: Ziming Li
//     Created: 2021-10-18
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/animation/LengthPairStyleInterpolation.h"

#include "core/animation/LengthStyleInterpolation.h"
#include "core/css/CSSValuePair.h"
#include "core/css/resolver/StyleBuilder.h"

namespace blink {

bool LengthPairStyleInterpolation::canCreateFrom(const CSSValue& value)
{
    return value.isValuePair();
}

PassOwnPtr<InterpolableValue> LengthPairStyleInterpolation::lengthPairToInterpolableValue(const CSSValue& value)
{
    OwnPtr<InterpolableList> result = InterpolableList::create(2);
    const CSSValuePair& pair = toCSSValuePair(value);

    result->set(0, LengthStyleInterpolation::toInterpolableValue(pair.first()));
    result->set(1, LengthStyleInterpolation::toInterpolableValue(pair.second()));
    return result.release();
}

GCRefPtr<CSSValue> LengthPairStyleInterpolation::interpolableValueToLengthPair(InterpolableValue* value, InterpolationRange range)
{
    InterpolableList* lengthPair = toInterpolableList(value);
    GCRefPtr<CSSPrimitiveValue> first = LengthStyleInterpolation::fromInterpolableValue(*lengthPair->get(0), range);
    GCRefPtr<CSSPrimitiveValue> second = LengthStyleInterpolation::fromInterpolableValue(*lengthPair->get(1), range);
    return CSSValuePair::create(first, second, CSSValuePair::KeepIdenticalValues);
}

void LengthPairStyleInterpolation::apply(StyleResolverState& state) const
{
    StyleBuilder::applyProperty(m_id, state, interpolableValueToLengthPair(m_cachedValue.get(), m_range).get());
}

} // namespace blink
