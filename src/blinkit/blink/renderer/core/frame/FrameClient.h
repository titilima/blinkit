#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: FrameClient.h
// Description: FrameClient Class
//      Author: Ziming Li
//     Created: 2021-06-28
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_FRAME_CLIENT_H
#define BLINKIT_BLINKIT_FRAME_CLIENT_H

#include "platform/heap/Handle.h"

namespace blink {

class FrameClient
{
public:
    virtual ~FrameClient(void) = default;
};

} // namespace blink

#endif // BLINKIT_BLINKIT_FRAME_CLIENT_H
