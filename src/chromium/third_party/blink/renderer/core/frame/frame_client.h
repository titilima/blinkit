// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: frame_client.h
// Description: FrameClient Class
//      Author: Ziming Li
//     Created: 2019-09-13
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BLINKIT_BLINK_FRAME_CLIENT_H
#define BLINKIT_BLINK_FRAME_CLIENT_H

#include "third_party/blink/renderer/platform/heap/garbage_collected.h"

namespace blink {

class Frame;

class FrameClient : public GarbageCollectedFinalized<FrameClient>
{
public:
    virtual ~FrameClient(void) = default;
};

}  // namespace blink

#endif  // BLINKIT_BLINK_FRAME_CLIENT_H
