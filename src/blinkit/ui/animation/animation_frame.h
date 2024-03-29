#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: animation_frame.h
// Description: AnimationFrame Class
//      Author: Ziming Li
//     Created: 2022-02-10
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_ANIMATION_FRAME_H
#define BLINKIT_ANIMATION_FRAME_H

#include "blinkit/blink/renderer/platform/geometry/int_size.h"
#include "third_party/skia/include/core/SkBitmap.h"

class SkCanvas;

namespace BlinKit {

class AnimationFrame
{
public:
    AnimationFrame(const IntSize &size);
    ~AnimationFrame(void);

    SkBitmap& GetBitmap(void) { return m_bitmap; }
    const SkBitmap& GetBitmap(void) const { return m_bitmap; }

    IntSize GetSize(void) const;

    SkCanvas BeginPaint(void) const;

#ifdef _Z_OS_WINDOWS
    operator HBITMAP() const { return m_hBitmap; }
#endif
private:
    SkBitmap m_bitmap;

#ifdef _Z_OS_WINDOWS
    static void ReleaseBitmap(void *, void *context);
    HBITMAP m_hBitmap = nullptr;
#endif
};

} // namespace BlinKit

#endif // BLINKIT_ANIMATION_FRAME_H
