// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: IdleDeadline.cpp
// Description: IdleDeadline Class
//      Author: Ziming Li
//     Created: 2021-07-21
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/dom/IdleDeadline.h"

// BKTODO: #include "core/timing/PerformanceBase.h"
#include "wtf/CurrentTime.h"

namespace blink {

IdleDeadline::IdleDeadline(double deadlineSeconds, CallbackType callbackType)
    : m_deadlineSeconds(deadlineSeconds)
    , m_callbackType(callbackType)
{
}

double IdleDeadline::timeRemaining() const
{
    double timeRemaining = m_deadlineSeconds - monotonicallyIncreasingTime();
    if (timeRemaining < 0)
        timeRemaining = 0;

    // BKTODO: return 1000.0 * PerformanceBase::clampTimeResolution(timeRemaining);
    return 0;
}

} // namespace blink
