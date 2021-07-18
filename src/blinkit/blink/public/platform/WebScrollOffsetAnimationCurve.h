// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: WebScrollOffsetAnimationCurve.h
// Description: WebScrollOffsetAnimationCurve Class
//      Author: Ziming Li
//     Created: 2021-07-17
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WebScrollOffsetAnimationCurve_h
#define WebScrollOffsetAnimationCurve_h

#include "WebCompositorAnimationCurve.h"
#include "blinkit/blink/renderer/platform/geometry/FloatPoint.h"

namespace blink {

class WebScrollOffsetAnimationCurve : public WebCompositorAnimationCurve {
public:
    virtual ~WebScrollOffsetAnimationCurve() { }

    enum ScrollDurationBehavior {
        ScrollDurationDeltaBased = 0,
        ScrollDurationConstant,
        ScrollDurationInverseDelta
    };

    virtual void setInitialValue(FloatPoint) = 0;
    virtual FloatPoint getValue(double time) const = 0;
    virtual double duration() const = 0;
    virtual FloatPoint targetValue() const = 0;
    virtual void updateTarget(double time, FloatPoint newTarget) = 0;
};

} // namespace blink

#endif // WebScrollOffsetAnimationCurve_h
