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
#ifdef _Z_OS_WINDOWS
    AnimationFrame(HDC hdc, const IntSize &size);
#endif
    ~AnimationFrame(void);

    SkBitmap& GetBitmap(void) { return m_bitmap; }
    const SkBitmap& GetBitmap(void) const { return m_bitmap; }

    SkCanvas* GetCanvas(void) const
    {
        ASSERT(m_canvas);
        return m_canvas.get();
    }
    IntSize GetSize(void) const;

#ifdef _Z_OS_WINDOWS
    operator HBITMAP() const { return m_hBitmap; }
#endif
private:
    SkBitmap m_bitmap;
    std::unique_ptr<SkCanvas> m_canvas;

#ifdef _Z_OS_WINDOWS
    static void ReleaseBitmap(void *, void *context);
    HBITMAP m_hBitmap = nullptr;
#endif
};

} // namespace BlinKit

#endif // BLINKIT_ANIMATION_FRAME_H
