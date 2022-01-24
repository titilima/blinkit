#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: clip_item.h
// Description: ClipItem Class
//      Author: Ziming Li
//     Created: 2022-01-05
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_CLIP_ITEM_H
#define BLINKIT_CLIP_ITEM_H

#include "blinkit/ui/compositor/playback/display_item.h"

class SkRRect;

namespace BlinKit {

class ClipItem final : public DisplayItem
{
public:
    ClipItem(const IntRect &visualRect, const IntRect &clipRect, const std::vector<SkRRect> &roundedClipRects);
    ~ClipItem(void) override;
private:
    void Playback(SkCanvas &canvas) override;

    const IntRect m_clipRect;
    const std::vector<SkRRect> m_roundedClipRects;
};

} // namespace BlinKit

#endif // BLINKIT_CLIP_ITEM_H
