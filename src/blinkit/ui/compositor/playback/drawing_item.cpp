// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: drawing_item.cpp
// Description: DrawingItem Class
//      Author: Ziming Li
//     Created: 2021-12-22
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./drawing_item.h"

#include "third_party/skia/include/core/SkPicture.h"

namespace BlinKit {

DrawingItem::DrawingItem(const IntRect &visualRect, const SkPicture *picture) : m_visualRect(visualRect), m_picture(picture)
{
}

DrawingItem::~DrawingItem(void) = default;

void DrawingItem::Playback(SkCanvas *canvas)
{
    m_picture->playback(canvas);
}

} // namespace BlinKit
