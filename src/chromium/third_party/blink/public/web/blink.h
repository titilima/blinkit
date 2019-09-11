// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: blink.h
// Description: Blink Initializer
//      Author: Ziming Li
//     Created: 2019-09-11
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_BLINK_H
#define BLINKIT_BLINK_BLINK_H

#pragma once

#include "public/platform/platform.h"

namespace blink {

void Initialize(Platform *platform, scheduler::WebThreadScheduler *mainThreadScheduler);

} // namespace blink

#endif // BLINKIT_BLINK_BLINK_H
