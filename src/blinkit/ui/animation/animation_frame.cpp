// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: animation_frame.cpp
// Description: AnimationFrame Class
//      Author: Ziming Li
//     Created: 2022-02-12
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#include "./animation_frame.h"

#include "third_party/skia/include/core/SkCanvas.h"

namespace BlinKit {

AnimationFrame::~AnimationFrame(void) = default;

SkCanvas AnimationFrame::BeginPaint(void) const
{
    return SkCanvas(m_bitmap);
}

IntSize AnimationFrame::GetSize(void) const
{
    const SkImageInfo &info = m_bitmap.info();
    return IntSize(info.width(), info.height());
}

#ifdef _Z_OS_WINDOWS
AnimationFrame::AnimationFrame(const IntSize &size)
{
    BITMAPINFOHEADER bih = { 0 };
    bih.biSize = sizeof(bih);
    bih.biWidth = size.width();
    bih.biHeight = -size.height(); // top-down
    bih.biPlanes = 1;
    bih.biBitCount = 32;
    bih.biCompression = BI_RGB;
    bih.biXPelsPerMeter = bih.biYPelsPerMeter = 1;

    PVOID bits = nullptr;
    m_hBitmap = CreateDIBSection(nullptr, reinterpret_cast<BITMAPINFO *>(&bih), DIB_RGB_COLORS, &bits, nullptr, 0);
    if (nullptr == m_hBitmap)
    {
        ASSERT(nullptr != m_hBitmap);
        return;
    }

    const SkImageInfo info = SkImageInfo::MakeN32(size.width(), size.height(), kOpaque_SkAlphaType);
    m_bitmap.installPixels(info, bits, info.minRowBytes(), nullptr, ReleaseBitmap, m_hBitmap);
}

void AnimationFrame::ReleaseBitmap(void *, void *context)
{
    HBITMAP hBitmap = reinterpret_cast<HBITMAP>(context);
    DeleteObject(hBitmap);
}
#endif // _Z_OS_WINDOWS

} // namespace BlinKit
