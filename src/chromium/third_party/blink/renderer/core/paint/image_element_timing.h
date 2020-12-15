// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: image_element_timing.h
// Description: ImageElementTiming Class
//      Author: Ziming Li
//     Created: 2020-10-01
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_PAINT_IMAGE_ELEMENT_TIMING_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_PAINT_IMAGE_ELEMENT_TIMING_H_

// BKTODO: #include "third_party/blink/public/platform/web_layer_tree_view.h"
#include "third_party/blink/renderer/core/frame/local_dom_window.h"
#include "third_party/blink/renderer/platform/supplementable.h"
// BKTODO: #include "third_party/blink/renderer/platform/wtf/functional.h"
#include "third_party/blink/renderer/platform/wtf/hash_set.h"
#include "third_party/blink/renderer/platform/wtf/time.h"
#include "third_party/blink/renderer/platform/wtf/vector.h"

namespace blink {

class HTMLImageElement;
class LayoutImage;

// ImageElementTiming is responsible for tracking the paint timings for <img>
// elements for a given window.
class CORE_EXPORT ImageElementTiming final
    : public GarbageCollectedFinalized<ImageElementTiming>,
      public Supplement<LocalDOMWindow> {
  USING_GARBAGE_COLLECTED_MIXIN(ImageElementTiming);

 public:
  static const char kSupplementName[];

  virtual ~ImageElementTiming() = default;

  static ImageElementTiming& From(LocalDOMWindow&);

#if 0 // BKTODO:
  // Called when the LayoutImage has been painted. This method might queue a
  // swap promise to compute and report paint timestamps.
  void NotifyImagePainted(const HTMLImageElement*,
                          const LayoutImage*,
                          const PaintLayer*);
#endif

  // Called when the LayoutImage will be destroyed.
  void NotifyWillBeDestroyed(const LayoutImage*);

 private:
  explicit ImageElementTiming(LocalDOMWindow&);

  void* GCObject(void) override { return this; }
#if 0 // BKTODO:
  // Callback for the swap promise. Reports paint timestamps.
  void ReportImagePaintSwapTime(WebLayerTreeView::SwapResult,
                                base::TimeTicks timestamp);

  // Struct containing information about image element timing.
  struct ElementTimingInfo {
    AtomicString name;
    IntRect rect;
    ElementTimingInfo(AtomicString name, IntRect rect)
        : name(name), rect(rect) {}
  };
  // Vector containing the element timing infos that will be reported during the
  // next swap promise callback.
  WTF::Vector<ElementTimingInfo> element_timings_;
#endif
  // Hashmap of LayoutImage objects for which paint has already been notified.
  WTF::HashSet<const LayoutImage*> images_notified_;

  DISALLOW_COPY_AND_ASSIGN(ImageElementTiming);
};

}  // namespace blink

#endif
