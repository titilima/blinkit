// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/animation/CSSValueInterpolationType.h"

#include "core/animation/InterpolationEnvironment.h"
#include "core/animation/StringKeyframe.h"
#include "core/css/resolver/StyleBuilder.h"

namespace blink {

class CSSValueNonInterpolableValue : public NonInterpolableValue {
public:
    ~CSSValueNonInterpolableValue() final { }

    static PassRefPtr<CSSValueNonInterpolableValue> create(PassRefPtrWillBeRawPtr<CSSValue> cssValue)
    {
        return adoptRef(new CSSValueNonInterpolableValue(cssValue));
    }

    CSSValue* cssValue() const { return m_cssValue.get(); }

    DECLARE_NON_INTERPOLABLE_VALUE_TYPE();

private:
    CSSValueNonInterpolableValue(PassRefPtrWillBeRawPtr<CSSValue> cssValue)
        : m_cssValue(cssValue)
    {
        ASSERT(m_cssValue);
    }

    RefPtrWillBePersistent<CSSValue> m_cssValue;
};

DEFINE_NON_INTERPOLABLE_VALUE_TYPE(CSSValueNonInterpolableValue);
DEFINE_NON_INTERPOLABLE_VALUE_TYPE_CASTS(CSSValueNonInterpolableValue);

PassOwnPtr<InterpolationValue> CSSValueInterpolationType::maybeConvertSingle(const PropertySpecificKeyframe& keyframe, const InterpolationEnvironment&, const UnderlyingValue&, ConversionCheckers&) const
{
    if (keyframe.isNeutral())
        return nullptr;

    return InterpolationValue::create(*this, InterpolableList::create(0), CSSValueNonInterpolableValue::create(toCSSPropertySpecificKeyframe(keyframe).value()));
}

void CSSValueInterpolationType::apply(const InterpolableValue&, const NonInterpolableValue* nonInterpolableValue, InterpolationEnvironment& environment) const
{
    StyleBuilder::applyProperty(cssProperty(), environment.state(), toCSSValueNonInterpolableValue(nonInterpolableValue)->cssValue());
}

} // namespace blink
