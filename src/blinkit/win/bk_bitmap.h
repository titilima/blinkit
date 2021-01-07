// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: bk_bitmap.h
// Description: BkBitmap Class
//      Author: Ziming Li
//     Created: 2018-07-01
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_BK_BITMAP_H
#define BLINKIT_BLINKIT_BK_BITMAP_H

#pragma once

#include <Windows.h>
#include <SkBitmap.h>

namespace BlinKit {

class BkBitmap final : public SkBitmap
{
public:
    BkBitmap(void) = default;

    HBITMAP InstallDIBSection(int width, int height, HDC hdc = nullptr);
private:
    static void OnFinalize(void *, void *context);
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_BK_BITMAP_H
