#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: transform_item.h
// Description: TransformItem Class
//      Author: Ziming Li
//     Created: 2022-01-24
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_TRANSFORM_ITEM_H
#define BLINKIT_TRANSFORM_ITEM_H

#include "blinkit/ui/compositor/playback/display_item.h"
#include "third_party/skia/include/utils/SkMatrix44.h"

namespace BlinKit {

class TransformItem final : public DisplayItem
{
public:
    TransformItem(const IntRect &visualRect, const SkMatrix44 &matrix);
private:
    void Playback(SkCanvas &canvas) override;

    const SkMatrix44 m_matrix;
};

} // namespace BlinKit

#endif // BLINKIT_TRANSFORM_ITEM_H
