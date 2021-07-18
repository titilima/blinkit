// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WebDisplayItemList_h
#define WebDisplayItemList_h

#include "WebBlendMode.h"
#include "WebFloatPoint.h"
#include "WebFloatRect.h"
#include "WebRect.h"
#include "WebSize.h"
#include "WebVector.h"

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
    virtual void appendDrawingItem(const WebRect& visualRect, const SkPicture*) { }

    virtual void appendClipItem(const WebRect& visualRect, const WebRect& clipRect, const WebVector<SkRRect>& roundedClipRects) { }
    virtual void appendEndClipItem(const WebRect& visualRect) { }
    virtual void appendClipPathItem(const WebRect& visualRect, const SkPath&, SkRegion::Op, bool antialias) { }
    virtual void appendEndClipPathItem(const WebRect& visualRect) { }
    virtual void appendFloatClipItem(const WebRect& visualRect, const WebFloatRect& clipRect) { }
    virtual void appendEndFloatClipItem(const WebRect& visualRect) { }
    virtual void appendTransformItem(const WebRect& visualRect, const SkMatrix44&) { }
    virtual void appendEndTransformItem(const WebRect& visualRect) { }
    virtual void appendCompositingItem(const WebRect& visualRect, float opacity,
        SkXfermode::Mode, SkRect* bounds, SkColorFilter*) { }
    virtual void appendEndCompositingItem(const WebRect& visualRect) { }

    virtual void appendFilterItem(const WebRect& visualRect, const WebFilterOperations&, const WebFloatRect& bounds) { }
    virtual void appendEndFilterItem(const WebRect& visualRect) { }

    // Scroll containers are identified by an opaque pointer.
    using ScrollContainerId = const void*;
    virtual void appendScrollItem(const WebRect& visualRect, const WebSize& scrollOffset, ScrollContainerId) { }
    virtual void appendEndScrollItem(const WebRect& visualRect) { }
};

} // namespace blink

#endif // WebDisplayItemList_h
