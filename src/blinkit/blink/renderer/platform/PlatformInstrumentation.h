#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: PlatformInstrumentation.h
// Description: Placeholders for Platform
//      Author: Ziming Li
//     Created: 2021-07-28
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_PLATFORM_INSTRUMENTATION_H
#define BLINKIT_BLINKIT_PLATFORM_INSTRUMENTATION_H

namespace blink {

namespace PlatformInstrumentation {

inline void DoNothing(void) {}

} // namespace PlatformInstrumentation

} // namespace blink

#define didDecodeImage              DoNothing
#define didDecodeLazyPixelRef       DoNothing
#define didDrawLazyPixelRef(...)    DoNothing()
#define willDecodeImage(...)        DoNothing()
#define willDecodeLazyPixelRef(...) DoNothing()

#endif // BLINKIT_BLINKIT_PLATFORM_INSTRUMENTATION_H
