// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: InputDeviceCapabilities.cpp
// Description: InputDeviceCapabilities Class
//      Author: Ziming Li
//     Created: 2021-09-26
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/input/InputDeviceCapabilities.h"

namespace blink {

InputDeviceCapabilities::InputDeviceCapabilities(bool firesTouchEvents)
{
    m_firesTouchEvents = firesTouchEvents;
}

InputDeviceCapabilities::InputDeviceCapabilities(const InputDeviceCapabilitiesInit& initializer)
{
    m_firesTouchEvents = initializer.firesTouchEvents();
}

InputDeviceCapabilities::~InputDeviceCapabilities()
{
}

InputDeviceCapabilities* InputDeviceCapabilities::firesTouchEventsSourceCapabilities()
{
    static InputDeviceCapabilities instance(true);
    return &instance;
}

InputDeviceCapabilities* InputDeviceCapabilities::doesntFireTouchEventsSourceCapabilities()
{
    static InputDeviceCapabilities instance(false);
    return &instance;
}

} // namespace blink
