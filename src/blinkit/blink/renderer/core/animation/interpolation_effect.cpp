// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: interpolation_effect.cpp
// Description: InterpolationEffect Class
//      Author: Ziming Li
//     Created: 2022-02-28
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "./interpolation_effect.h"

namespace blink {

void InterpolationEffect::getActiveInterpolations(double fraction, double iterationDuration, std::vector<RefPtr<Interpolation>>& result) const
{
    size_t existingSize = result.size();
    size_t resultIndex = 0;

    for (const auto& record : m_interpolations) {
        if (fraction >= record->m_applyFrom && fraction < record->m_applyTo) {
            RefPtr<Interpolation> interpolation = record->m_interpolation;
            double localFraction = (fraction - record->m_start) / (record->m_end - record->m_start);
            if (record->m_easing)
                localFraction = record->m_easing->evaluate(localFraction, accuracyForDuration(iterationDuration));
            interpolation->interpolate(0, localFraction);
            if (resultIndex < existingSize)
                result[resultIndex++] = interpolation;
            else
                result.emplace_back(interpolation);
        }
    }
    if (resultIndex < existingSize)
        result.resize(resultIndex);
}

void InterpolationEffect::addInterpolationsFromKeyframes(PropertyHandle property, Element* element, const ComputedStyle* baseStyle, Keyframe::PropertySpecificKeyframe& keyframeA, Keyframe::PropertySpecificKeyframe& keyframeB, double applyFrom, double applyTo)
{
    RefPtr<Interpolation> interpolation = keyframeA.maybeCreateInterpolation(property, keyframeB, element, baseStyle);

    if (interpolation) {
        addInterpolation(interpolation, &keyframeA.easing(), keyframeA.offset(), keyframeB.offset(), applyFrom, applyTo);
    } else {
        RefPtr<Interpolation> interpolationA = keyframeA.maybeCreateInterpolation(property, keyframeA, element, baseStyle);
        RefPtr<Interpolation> interpolationB = keyframeB.maybeCreateInterpolation(property, keyframeB, element, baseStyle);

        Vector<TimingFunction::PartitionRegion> regions = Vector<TimingFunction::PartitionRegion>();
        keyframeA.easing().partition(regions);

        size_t regionIndex = 0;
        for (const auto& region : regions) {
            double regionStart = blend(keyframeA.offset(), keyframeB.offset(), region.start);
            double regionEnd = blend(keyframeA.offset(), keyframeB.offset(), region.end);

            double regionApplyFrom = regionIndex == 0 ? applyFrom : regionStart;
            double regionApplyTo = regionIndex == regions.size() - 1 ? applyTo : regionEnd;

            if (region.half == TimingFunction::RangeHalf::Lower) {
                interpolation = interpolationA;
            } else if (region.half == TimingFunction::RangeHalf::Upper) {
                interpolation = interpolationB;
            } else {
                ASSERT_NOT_REACHED();
                continue;
            }

            if (interpolation) {
                addInterpolation(interpolation.release(),
                    &keyframeA.easing(), regionStart, regionEnd, regionApplyFrom, regionApplyTo);
            }

            regionIndex++;
        }
    }
}

} // namespace blink
