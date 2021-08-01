// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: WebDisplayItemList.h
// Description: WebDisplayItemList Class
//      Author: Ziming Li
//     Created: 2021-07-18
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WebDisplayItemList_h
#define WebDisplayItemList_h

#include "WebBlendMode.h"
#include "blinkit/blink/renderer/platform/geometry/FloatRect.h"
#include "blinkit/blink/renderer/platform/geometry/IntRect.h"
#include "third_party/skia/include/core/SkColorFilter.h"
#include "third_party/skia/include/core/SkRRect.h"
#include "third_party/skia/include/core/SkRegion.h"
#include "third_party/skia/include/core/SkXfermode.h"
#include "third_party/skia/include/utils/SkMatrix44.h"

class SkImageFilter;
class SkMatrix44;
class SkPicture;

namespace blink {
class WebFilterOperations;

// An ordered list of items representing content to be rendered (stored in
// 'drawing' items) and operations to be performed when rendering this content
// (stored in 'clip', 'transform', 'filter', etc...). For more details see:
// http://dev.chromium.org/blink/slimming-paint.
class WebDisplayItemList {
public:
    virtual ~WebDisplayItemList() { }

    // This grabs a ref on the passed-in SkPicture.
    virtual void appendDrawingItem(const IntRect& visualRect, const SkPicture*) { }

    virtual void appendClipItem(const IntRect& visualRect, const IntRect& clipRect, const std::vector<SkRRect>& roundedClipRects) { }
    virtual void appendEndClipItem(const IntRect& visualRect) { }
    virtual void appendClipPathItem(const IntRect& visualRect, const SkPath&, SkRegion::Op, bool antialias) { }
    virtual void appendEndClipPathItem(const IntRect& visualRect) { }
    virtual void appendFloatClipItem(const IntRect& visualRect, const FloatRect& clipRect) { }
    virtual void appendEndFloatClipItem(const IntRect& visualRect) { }
    virtual void appendTransformItem(const IntRect& visualRect, const SkMatrix44&) { }
    virtual void appendEndTransformItem(const IntRect& visualRect) { }
    virtual void appendCompositingItem(const IntRect& visualRect, float opacity,
        SkXfermode::Mode, SkRect* bounds, SkColorFilter*) { }
    virtual void appendEndCompositingItem(const IntRect& visualRect) { }

    virtual void appendFilterItem(const IntRect& visualRect, const WebFilterOperations&, const FloatRect& bounds) { }
    virtual void appendEndFilterItem(const IntRect& visualRect) { }

    // Scroll containers are identified by an opaque pointer.
    using ScrollContainerId = const void*;
    virtual void appendScrollItem(const IntRect& visualRect, const IntSize& scrollOffset, ScrollContainerId) { }
    virtual void appendEndScrollItem(const IntRect& visualRect) { }
};

} // namespace blink

#endif // WebDisplayItemList_h
