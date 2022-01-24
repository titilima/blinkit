// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: transform_item.cpp
// Description: TransformItem Class
//      Author: Ziming Li
//     Created: 2022-01-24
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#include "./transform_item.h"

namespace BlinKit {

TransformItem::TransformItem(const IntRect &visualRect, const SkMatrix44 &matrix)
    : DisplayItem(visualRect), m_matrix(matrix)
{
}

void TransformItem::Playback(SkCanvas &canvas)
{
    canvas.save();
    if (!m_matrix.isIdentity())
        canvas.concat(m_matrix);
}

} // namespace BlinKit
