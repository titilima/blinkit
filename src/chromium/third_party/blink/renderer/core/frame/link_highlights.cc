// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: link_highlights.cc
// Description: LinkHighlights Class
//      Author: Ziming Li
//     Created: 2020-12-20
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/frame/link_highlights.h"

#include <memory>

// BKTODO: #include "cc/layers/picture_layer.h"
#include "third_party/blink/public/platform/platform.h"
// BKTODO: #include "third_party/blink/public/platform/web_layer_tree_view.h"
#include "third_party/blink/renderer/core/dom/node.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/layout/layout_object.h"
#include "third_party/blink/renderer/core/page/chrome_client.h"
#include "third_party/blink/renderer/core/page/page.h"
#include "third_party/blink/renderer/core/paint/link_highlight_impl.h"
#if 0 // BKTODO:
#include "third_party/blink/renderer/platform/animation/compositor_animation_host.h"
#endif
#include "third_party/blink/renderer/platform/animation/compositor_animation_timeline.h"

namespace blink {

LinkHighlights::LinkHighlights(Page& owner) : page_(&owner) {}

LinkHighlights::~LinkHighlights() {
  RemoveAllHighlights();
}

void LinkHighlights::RemoveAllHighlights() {
  for (auto& highlight : link_highlights_) {
    ASSERT(false); // BKTODO:
#if 0
    if (timeline_)
      timeline_->AnimationDestroyed(*highlight);
    if (auto* node = highlight->GetNode()) {
      if (auto* layout_object = node->GetLayoutObject())
        layout_object->SetNeedsPaintPropertyUpdate();
    }
#endif
  }
  link_highlights_.clear();
}

void LinkHighlights::ResetForPageNavigation() {
  RemoveAllHighlights();
}

void LinkHighlights::SetTapHighlights(
    HeapVector<Member<Node>>& highlight_nodes) {
  // Always clear any existing highlight when this is invoked, even if we
  // don't get a new target to highlight.
  RemoveAllHighlights();

  for (wtf_size_t i = 0; i < highlight_nodes.size(); ++i) {
    Node* node = highlight_nodes[i];

    if (!node || !node->GetLayoutObject())
      continue;

    Color highlight_color =
        node->GetLayoutObject()->StyleRef().TapHighlightColor();
    // Safari documentation for -webkit-tap-highlight-color says if the
    // specified color has 0 alpha, then tap highlighting is disabled.
    // http://developer.apple.com/library/safari/#documentation/appleapplications/reference/safaricssref/articles/standardcssproperties.html
    if (!highlight_color.Alpha())
      continue;

    ASSERT(false); // BKTODO:
#if 0
    link_highlights_.push_back(LinkHighlightImpl::Create(node));
    if (timeline_)
      timeline_->AnimationAttached(*link_highlights_.back());
#endif
    node->GetLayoutObject()->SetNeedsPaintPropertyUpdate();
  }
}

void LinkHighlights::UpdateGeometry() {
  for (auto& highlight : link_highlights_)
    ASSERT(false); // BKTODO: highlight->UpdateGeometry();
}

LocalFrame* LinkHighlights::MainFrame() const {
  return GetPage().MainFrame();
}

void LinkHighlights::StartHighlightAnimationIfNeeded() {
  for (auto& highlight : link_highlights_)
    ASSERT(false); // BKTODO: highlight->StartHighlightAnimationIfNeeded();

  if (auto* local_frame = MainFrame())
    GetPage().GetChromeClient().ScheduleAnimation(local_frame->View());
}

void LinkHighlights::LayerTreeViewInitialized(
    WebLayerTreeView& layer_tree_view) {
#if 0 // BKTODO: Check the logic later
  if (Platform::Current()->IsThreadedAnimationEnabled()) {
    timeline_ = CompositorAnimationTimeline::Create();
    animation_host_ = std::make_unique<CompositorAnimationHost>(
        layer_tree_view.CompositorAnimationHost());
    animation_host_->AddTimeline(*timeline_);
  }
#endif
}

void LinkHighlights::WillCloseLayerTreeView(WebLayerTreeView& layer_tree_view) {
  RemoveAllHighlights();
  if (timeline_) {
    ASSERT(false); // BKTODO: animation_host_->RemoveTimeline(*timeline_);
    timeline_.reset();
  }
  // BKTODO: animation_host_ = nullptr;
}

bool LinkHighlights::NeedsHighlightEffectInternal(
    const LayoutObject& object) const {
  for (auto& highlight : link_highlights_) {
    ASSERT(false); // BKTODO:
#if 0
    if (auto* node = highlight->GetNode()) {
      if (node->GetLayoutObject() == &object)
        return true;
    }
#endif
  }
  return false;
}

CompositorElementId LinkHighlights::element_id(const LayoutObject& object) {
  for (auto& highlight : link_highlights_) {
    ASSERT(false); // BKTODO:
#if 0
    if (auto* node = highlight->GetNode()) {
      if (node->GetLayoutObject() == &object)
        return highlight->element_id();
    }
#endif
  }
  return CompositorElementId();
}

}  // namespace blink
