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

#include "blinkit/ui/compositor/playback/drawing_item.h"

namespace BlinKit {

DisplayItemList::DisplayItemList(void) = default;
DisplayItemList::~DisplayItemList(void) = default;

void DisplayItemList::appendDrawingItem(const IntRect &visualRect, const SkPicture *picture)
{
    m_displayItems.emplace_back(std::make_unique<DrawingItem>(visualRect, picture));
}

void DisplayItemList::Playback(SkCanvas *canvas) const
{
    for (const auto &item : m_displayItems)
        item->Playback(canvas);
}

} // namespace BlinKit
