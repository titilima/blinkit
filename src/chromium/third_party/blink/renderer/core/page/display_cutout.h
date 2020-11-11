// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: display_cutout.h
// Description: DisplayCutout Stuff
//      Author: Ziming Li
//     Created: 2020-10-30
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_DISPLAY_CUTOUT_H
#define BLINKIT_BLINK_DISPLAY_CUTOUT_H

#pragma once

namespace blink {

enum class ViewportFit {
    kAuto = 0,
    kContain,
    kCover,
    kCoverForcedByUserAgent,

    kMinValue = kAuto,
    kMaxValue = kCoverForcedByUserAgent,
};

} // namespace blink

namespace mojom {
using blink::ViewportFit;
} // namespace mojom

#endif // BLINKIT_BLINK_DISPLAY_CUTOUT_H
