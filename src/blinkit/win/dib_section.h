// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: dib_section.h
// Description: DIBSection Class
//      Author: Ziming Li
//     Created: 2018-07-01
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_DIB_SECTION_H
#define BLINKIT_BLINKIT_DIB_SECTION_H

#pragma once

#include <SkBitmap.h>

namespace BlinKit {

class DIBSection final : public SkBitmap
{
public:
    DIBSection(int width, int height, HDC hdc = nullptr);

    HBITMAP GetHBITMAP(void) { return m_hBitmap; }
private:
    static void OnFinalize(void *, void *context);

    HBITMAP m_hBitmap = nullptr;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_DIB_SECTION_H
