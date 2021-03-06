// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: paint_layer_resource_info.h
// Description: PaintLayerResourceInfo Class
//      Author: Ziming Li
//     Created: 2020-08-29
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2012 Adobe Systems Incorporated. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer.
 * 2. Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_PAINT_PAINT_LAYER_RESOURCE_INFO_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_PAINT_PAINT_LAYER_RESOURCE_INFO_H_

#include "base/macros.h"
// BKTODO: #include "third_party/blink/renderer/core/svg/svg_resource_client.h"
#include "third_party/blink/renderer/platform/heap/handle.h"

namespace blink {

class FilterEffect;
class PaintLayer;

// PaintLayerResourceInfo holds the filter information for painting
// https://drafts.fxtf.org/filter-effects/. It also acts as the resource client
// for change notifications from <clipPath> elements for the clip-path property.
//
// Because PaintLayer is not allocated for SVG objects, SVG filters (both
// software and hardware-accelerated) use a different code path to paint the
// filters (SVGFilterPainter), but both code paths use the same abstraction for
// painting non-hardware accelerated filters (FilterEffect). Hardware
// accelerated CSS filters use CompositorFilterOperations, that is backed by cc.
class PaintLayerResourceInfo final
    : public GarbageCollectedFinalized<PaintLayerResourceInfo> {
      // BKTODO: public SVGResourceClient {
  USING_GARBAGE_COLLECTED_MIXIN(PaintLayerResourceInfo);

 public:
  explicit PaintLayerResourceInfo(PaintLayer*);
  ~PaintLayerResourceInfo();

  void SetLastEffect(FilterEffect*);
  FilterEffect* LastEffect() const;
  void InvalidateFilterChain();

  void ClearLayer() { layer_ = nullptr; }

#if 0
  void ResourceContentChanged(InvalidationModeMask) override;
  void ResourceElementChanged() override;
#endif

  void Trace(blink::Visitor*);

 private:
  // |clearLayer| must be called before *m_layer becomes invalid.
  PaintLayer* layer_;
  Member<FilterEffect> last_effect_;
  DISALLOW_COPY_AND_ASSIGN(PaintLayerResourceInfo);
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_PAINT_PAINT_LAYER_RESOURCE_INFO_H_
