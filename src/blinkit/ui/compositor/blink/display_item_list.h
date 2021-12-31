#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: display_item_list.h
// Description: DisplayItemList Class
//      Author: Ziming Li
//     Created: 2021-12-21
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_DISPLAY_ITEM_LIST_H
#define BLINKIT_DISPLAY_ITEM_LIST_H

#include "blinkit/blink/public/platform/WebDisplayItemList.h"
#include "blinkit/ui/compositor/playback/display_item.h"

class SkCanvas;

namespace BlinKit {

class DisplayItemList : public WebDisplayItemList
{
public:
    ~DisplayItemList(void) override;

    void Playback(SkCanvas *canvas) const;
protected:
    DisplayItemList(void);
private:
    void appendDrawingItem(const IntRect &visualRect, const SkPicture *picture) override;
    void appendClipItem(const IntRect& visualRect, const IntRect& clipRect, const std::vector<SkRRect>& roundedClipRects) override {
        ASSERT(false); // BKTODO:
    }
    void appendEndClipItem(const IntRect& visualRect) override {
        ASSERT(false); // BKTODO:
    }
    void appendClipPathItem(const IntRect& visualRect, const SkPath&, SkRegion::Op, bool antialias) override {
        ASSERT(false); // BKTODO:
    }
    void appendEndClipPathItem(const IntRect& visualRect) override {
        ASSERT(false); // BKTODO:
    }
    void appendFloatClipItem(const IntRect& visualRect, const FloatRect& clipRect) override {
        ASSERT(false); // BKTODO:
    }
    void appendEndFloatClipItem(const IntRect& visualRect) override {
        ASSERT(false); // BKTODO:
    }
    void appendTransformItem(const IntRect& visualRect, const SkMatrix44&) override {
        ASSERT(false); // BKTODO:
    }
    void appendEndTransformItem(const IntRect& visualRect) override {
        ASSERT(false); // BKTODO:
    }
    void appendCompositingItem(const IntRect& visualRect, float opacity,
        SkXfermode::Mode, SkRect* bounds, SkColorFilter*) override {
        ASSERT(false); // BKTODO:
    }
    void appendEndCompositingItem(const IntRect& visualRect) override {
        ASSERT(false); // BKTODO:
    }
    void appendFilterItem(const IntRect& visualRect, const WebFilterOperations&, const FloatRect& bounds) override {
        ASSERT(false); // BKTODO:
    }
    void appendEndFilterItem(const IntRect& visualRect) override {
        ASSERT(false); // BKTODO:
    }
    void appendScrollItem(const IntRect& visualRect, const IntSize& scrollOffset, ScrollContainerId) override {
        ASSERT(false); // BKTODO:
    }
    void appendEndScrollItem(const IntRect& visualRect) override {
        ASSERT(false); // BKTODO:
    }
private:
    std::vector<std::unique_ptr<DisplayItem>> m_displayItems;
};

} // namespace BlinKit

#endif // BLINKIT_DISPLAY_ITEM_LIST_H
