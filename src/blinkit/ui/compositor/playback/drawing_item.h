#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: drawing_item.h
// Description: DrawingItem Class
//      Author: Ziming Li
//     Created: 2021-12-22
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_DRAWING_ITEM_H
#define BLINKIT_DRAWING_ITEM_H

#include "blinkit/blink/renderer/platform/geometry/IntRect.h"
#include "blinkit/blink/renderer/wtf/RefPtr.h"
#include "blinkit/ui/compositor/playback/display_item.h"

class SkPicture;

namespace BlinKit {

class DrawingItem final : public DisplayItem
{
public:
    DrawingItem(const IntRect &visualRect, const SkPicture *picture);
    ~DrawingItem(void) override;

    void Playback(SkCanvas *canvas) override;
private:
    const IntRect m_visualRect;
    RefPtr<const SkPicture> m_picture;
};

} // namespace BlinKit

#endif // BLINKIT_DRAWING_ITEM_H
