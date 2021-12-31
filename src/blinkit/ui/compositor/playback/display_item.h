#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: display_item.h
// Description: DisplayItem Class
//      Author: Ziming Li
//     Created: 2021-12-22
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_DISPLAY_ITEM_H
#define BLINKIT_DISPLAY_ITEM_H

class SkCanvas;

namespace BlinKit {

class DisplayItem
{
public:
    virtual ~DisplayItem(void) = default;

    virtual void Playback(SkCanvas *canvas) = 0;
protected:
    DisplayItem(void) = default;
};

} // namespace BlinKit

#endif // BLINKIT_DISPLAY_ITEM_H
