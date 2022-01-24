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

#include "blinkit/ui/compositor/playback/clip_item.h"
#include "blinkit/ui/compositor/playback/drawing_item.h"
#include "blinkit/ui/compositor/playback/transform_item.h"

namespace BlinKit {

void DisplayItemList::appendClipItem(const IntRect &visualRect, const IntRect &clipRect, const std::vector<SkRRect> &roundedClipRects)
{
    m_displayItems.emplace_back(std::make_unique<ClipItem>(visualRect, clipRect, roundedClipRects));
}

void DisplayItemList::appendDrawingItem(const IntRect &visualRect, const SkPicture *picture)
{
    m_displayItems.emplace_back(std::make_unique<DrawingItem>(visualRect, picture));
}

void DisplayItemList::appendScrollItem(const IntRect &visualRect, const IntSize &scrollOffset, ScrollContainerId)
{
    SkMatrix44 matrix(SkMatrix44::kUninitialized_Constructor);
    matrix.setTranslate(-scrollOffset.width(), -scrollOffset.height(), 0);
    // TODO(wkorman): Should we translate the visual rect as well?
    m_displayItems.emplace_back(std::make_unique<TransformItem>(visualRect, matrix));
}

void DisplayItemList::appendTransformItem(const IntRect &visualRect, const SkMatrix44 &matrix)
{
    m_displayItems.emplace_back(std::make_unique<TransformItem>(visualRect, matrix));
}

void DisplayItemList::Playback(SkCanvas &canvas) const
{
    for (const auto &item : m_displayItems)
        item->Playback(canvas);
}

} // namespace BlinKit
