// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: platform_instrumentation.h
// Description: Placeholders for Instrumentation
//      Author: Ziming Li
//     Created: 2021-01-11
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_PLATFORM_INSTRUMENTATION_H
#define BLINKIT_BLINK_PLATFORM_INSTRUMENTATION_H

#pragma once

namespace blink {
namespace PlatformInstrumentation {
inline void NothingToDo(void) {}
} // namespace PlatformInstrumentation
} // namespace blink

#define WillDecodeImage(...)    NothingToDo()
#define DidDecodeImage(...)     NothingToDo()

#endif // BLINKIT_BLINK_PLATFORM_INSTRUMENTATION_H
