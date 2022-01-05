// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: display_item_list.cpp
// Description: DisplayItemList Class
//      Author: Ziming Li
//     Created: 2021-12-21
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./display_item_list.h"

#include "blinkit/ui/compositor/playback/clip_items.h"
#include "blinkit/ui/compositor/playback/drawing_item.h"

namespace BlinKit {

void DisplayItemList::appendClipItem(const IntRect &visualRect, const IntRect &clipRect, const std::vector<SkRRect> &roundedClipRects)
{
    m_displayItems.emplace_back(std::make_unique<ClipItem>(visualRect, clipRect, roundedClipRects));
}

void DisplayItemList::appendDrawingItem(const IntRect &visualRect, const SkPicture *picture)
{
    m_displayItems.emplace_back(std::make_unique<DrawingItem>(visualRect, picture));
}

void DisplayItemList::appendEndClipItem(const IntRect &visualRect)
{
    m_displayItems.emplace_back(std::make_unique<EndClipItem>(visualRect));
}

void DisplayItemList::Playback(SkCanvas &canvas) const
{
    for (const auto &item : m_displayItems)
        item->Playback(canvas);
}

} // namespace BlinKit
