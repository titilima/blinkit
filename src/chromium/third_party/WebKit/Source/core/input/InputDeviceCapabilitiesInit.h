// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: InputDeviceCapabilitiesInit.h
// Description: InputDeviceCapabilitiesInit Class
//      Author: Ziming Li
//     Created: 2018-07-12
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_INPUT_DEVICE_CAPABILITIES_INIT_H
#define BLINKIT_BLINK_INPUT_DEVICE_CAPABILITIES_INIT_H

#pragma once

#include "platform/heap/Handle.h"

namespace blink {

class InputDeviceCapabilitiesInit
{
public:
    bool firesTouchEvents(void) const { return m_firesTouchEvents; }
private:
    bool m_firesTouchEvents = false;
};

} // namespace blink

#endif // BLINKIT_BLINK_INPUT_DEVICE_CAPABILITIES_INIT_H
