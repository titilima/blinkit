// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: bitmap_image_metrics.h
// Description: Placeholders for BitmapImageMetrics
//      Author: Ziming Li
//     Created: 2021-01-11
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_BITMAP_IMAGE_METRICS_H
#define BLINKIT_BLINK_BITMAP_IMAGE_METRICS_H

#pragma once

namespace blink {
namespace BitmapImageMetrics {
inline void NothingToDo(void) {}
} // namespace BitmapImageMetrics
} // namespace blink

#define CountDecodedImageType(...)      NothingToDo()
#define CountImageGammaAndGamut(...)    NothingToDo()
#define CountImageJpegDensity(...)      NothingToDo()
#define CountImageOrientation(...)      NothingToDo()

#endif // BLINKIT_BLINK_BITMAP_IMAGE_METRICS_H
