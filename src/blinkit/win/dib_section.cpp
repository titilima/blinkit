// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: dib_section.cpp
// Description: DIBSection Class
//      Author: Ziming Li
//     Created: 2018-07-01
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#include "dib_section.h"

namespace BlinKit {

DIBSection::DIBSection(int width, int height, HDC hdc)
{
    BITMAPINFOHEADER bih = { 0 };
    bih.biSize = sizeof(bih);
    bih.biWidth = width;
    bih.biHeight = -height; // top-down
    bih.biPlanes = 1;
    bih.biBitCount = 32;
    bih.biCompression = BI_RGB;
    bih.biXPelsPerMeter = bih.biYPelsPerMeter = 1;

    PVOID bits = nullptr;
    m_hBitmap = CreateDIBSection(hdc, reinterpret_cast<BITMAPINFO *>(&bih), 0, &bits, nullptr, 0);
    ASSERT(nullptr != m_hBitmap);

    if (nullptr != m_hBitmap)
    {
        const SkImageInfo info = SkImageInfo::MakeN32(width, height, kOpaque_SkAlphaType);
        installPixels(info, bits, info.minRowBytes(), OnFinalize, m_hBitmap);
    }
}

void DIBSection::OnFinalize(void *, void *context)
{
    HBITMAP hBitmap = reinterpret_cast<HBITMAP>(context);
    DeleteObject(hBitmap);
}

} // namespace BlinKit
