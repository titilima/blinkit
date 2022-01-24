// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: clip_items.cpp
// Description: ClipItem Class
//      Author: Ziming Li
//     Created: 2022-01-05
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#include "./clip_item.h"

#include "third_party/skia/include/core/SkRRect.h"

namespace BlinKit {

ClipItem::ClipItem(const IntRect &visualRect, const IntRect &clipRect, const std::vector<SkRRect> &roundedClipRects)
    : DisplayItem(visualRect), m_clipRect(clipRect), m_roundedClipRects(roundedClipRects)
{
}

ClipItem::~ClipItem(void) = default;

void ClipItem::Playback(SkCanvas &canvas)
{
    constexpr SkRegion::Op op = SkRegion::kIntersect_Op;
    constexpr bool antiAlias = true;

    canvas.save();
    canvas.clipRect(m_clipRect, op, antiAlias);
    for (const SkRRect &rrect : m_roundedClipRects)
    {
        if (rrect.isRect())
            canvas.clipRect(rrect.rect(), op, antiAlias);
        else
            canvas.clipRRect(rrect, op, antiAlias);
    }
}

} // namespace BlinKit
