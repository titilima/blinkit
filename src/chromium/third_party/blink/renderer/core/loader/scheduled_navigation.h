// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: scheduled_navigation.h
// Description: ScheduledNavigation Class
//      Author: Ziming Li
//     Created: 2019-10-08
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BLINKIT_BLINK_SCHEDULED_NAVIGATION_H
#define BLINKIT_BLINK_SCHEDULED_NAVIGATION_H

#include "base/macros.h"
#include "third_party/blink/renderer/platform/heap/handle.h"

namespace blink {

class ScheduledNavigation : public GarbageCollectedFinalized<ScheduledNavigation>
{
public:
    bool IsLocationChange(void) const { return m_isLocationChange; }
private:
    ScheduledNavigation(void) = delete; // BKTODO:

    bool m_isLocationChange = false;

    DISALLOW_COPY_AND_ASSIGN(ScheduledNavigation);
};

}  // namespace blink

#endif  // BLINKIT_BLINK_SCHEDULED_NAVIGATION_H
