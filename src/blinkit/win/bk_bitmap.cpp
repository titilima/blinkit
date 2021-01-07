// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: bk_bitmap.cpp
// Description: BkBitmap Class
//      Author: Ziming Li
//     Created: 2018-07-01
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#include "bk_bitmap.h"

namespace BlinKit {

HBITMAP BkBitmap::InstallDIBSection(int width, int height, HDC hdc)
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
    HBITMAP ret = CreateDIBSection(hdc, reinterpret_cast<BITMAPINFO *>(&bih), 0, &bits, nullptr, 0);
    ASSERT(nullptr != ret);
    if (nullptr != ret)
    {
        const SkImageInfo info = SkImageInfo::MakeN32(width, height, kOpaque_SkAlphaType);
        installPixels(info, bits, info.minRowBytes(), OnFinalize, ret);
    }
    return ret;
}

void BkBitmap::OnFinalize(void *, void *context)
{
    HBITMAP hBitmap = reinterpret_cast<HBITMAP>(context);
    DeleteObject(hBitmap);
}

} // namespace BlinKit
