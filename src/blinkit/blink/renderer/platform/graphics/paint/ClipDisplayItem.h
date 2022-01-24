#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: ClipDisplayItem.h
// Description: ClipDisplayItem Class
//      Author: Ziming Li
//     Created: 2022-01-23
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ClipDisplayItem_h
#define ClipDisplayItem_h

#include "blinkit/blink/renderer/platform/geometry/FloatRoundedRect.h"
#include "blinkit/blink/renderer/platform/geometry/int_rect.h"
#include "blinkit/blink/renderer/platform/graphics/paint/DisplayItem.h"
#include "blinkit/blink/renderer/wtf/PassOwnPtr.h"
#include "blinkit/blink/renderer/wtf/Vector.h"
#include "third_party/skia/include/core/SkRegion.h"

namespace blink {

class ClipDisplayItem final : public PairedBeginDisplayItem
{
public:
    ClipDisplayItem(const DisplayItemClient& client, Type type, const IntRect& clipRect)
        : PairedBeginDisplayItem(client, type, sizeof(*this))
        , m_clipRect(clipRect)
    {
        ASSERT(isClipType(type));
    }

    ClipDisplayItem(const DisplayItemClient& client, Type type, const IntRect& clipRect, Vector<FloatRoundedRect>& roundedRectClips)
        : ClipDisplayItem(client, type, clipRect)
    {
        m_roundedRectClips.swap(roundedRectClips);
    }

    void replay(GraphicsContext&) const override;
    void appendToWebDisplayItemList(const IntRect&, WebDisplayItemList*) const override;

private:
#ifndef NDEBUG
    void dumpPropertiesAsDebugString(WTF::StringBuilder&) const override;
#endif
#if ENABLE(ASSERT)
    bool equals(const DisplayItem& other) const final
    {
        return DisplayItem::equals(other)
            && m_clipRect == static_cast<const ClipDisplayItem&>(other).m_clipRect
            && m_roundedRectClips == static_cast<const ClipDisplayItem&>(other).m_roundedRectClips;
    }
#endif

    const IntRect m_clipRect;
    Vector<FloatRoundedRect> m_roundedRectClips;
};

class PLATFORM_EXPORT EndClipDisplayItem final : public PairedEndDisplayItem {
public:
    EndClipDisplayItem(const DisplayItemClient& client, Type type)
        : PairedEndDisplayItem(client, type, sizeof(*this))
    {
        ASSERT(isEndClipType(type));
    }

    void replay(GraphicsContext&) const override;
    void appendToWebDisplayItemList(const IntRect&, WebDisplayItemList*) const override;

private:
#if ENABLE(ASSERT)
    bool isEndAndPairedWith(DisplayItem::Type otherType) const final { return DisplayItem::isClipType(otherType); }
#endif
};

} // namespace blink

#endif // ClipDisplayItem_h
