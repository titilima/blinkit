// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: hit_test_result.cc
// Description: HitTestResult Class
//      Author: Ziming Li
//     Created: 2020-09-30
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2006, 2008, 2011 Apple Inc. All rights reserved.
 * Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

#include "third_party/blink/renderer/core/layout/hit_test_result.h"

#include "third_party/blink/renderer/core/dom/flat_tree_traversal.h"
#include "third_party/blink/renderer/core/dom/pseudo_element.h"
#include "third_party/blink/renderer/core/dom/shadow_root.h"
#include "third_party/blink/renderer/core/editing/editing_utilities.h"
#include "third_party/blink/renderer/core/editing/frame_selection.h"
#include "third_party/blink/renderer/core/editing/position_with_affinity.h"
#include "third_party/blink/renderer/core/editing/visible_units.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#if 0 // BKTODO:
#include "third_party/blink/renderer/core/html/forms/html_input_element.h"
#include "third_party/blink/renderer/core/html/forms/html_text_area_element.h"
#include "third_party/blink/renderer/core/html/html_area_element.h"
#include "third_party/blink/renderer/core/html/html_image_element.h"
#include "third_party/blink/renderer/core/html/html_map_element.h"
#include "third_party/blink/renderer/core/html/media/html_media_element.h"
#endif
#include "third_party/blink/renderer/core/html/parser/html_parser_idioms.h"
#include "third_party/blink/renderer/core/html_names.h"
#include "third_party/blink/renderer/core/input_type_names.h"
#include "third_party/blink/renderer/core/layout/layout_image.h"
#include "third_party/blink/renderer/core/scroll/scrollbar.h"
// BKTODO: #include "third_party/blink/renderer/core/svg/svg_element.h"
#include "third_party/blink/renderer/platform/geometry/region.h"
// BKTODO: #include "third_party/blink/renderer/platform/mediastream/media_stream_descriptor.h"

namespace blink {

using namespace html_names;

HitTestResult::HitTestResult()
    : hit_test_request_(HitTestRequest::kReadOnly | HitTestRequest::kActive),
      cacheable_(true),
      is_over_embedded_content_view_(false) {}

HitTestResult::HitTestResult(const HitTestRequest& other_request,
                             const HitTestLocation& location)
    : hit_test_request_(other_request),
      cacheable_(true),
      point_in_inner_node_frame_(location.Point()),
      is_over_embedded_content_view_(false) {}

HitTestResult::HitTestResult(const HitTestResult& other)
    : hit_test_request_(other.hit_test_request_),
      cacheable_(other.cacheable_),
      inner_node_(other.InnerNode()),
      inner_possibly_pseudo_node_(other.inner_possibly_pseudo_node_),
      point_in_inner_node_frame_(other.point_in_inner_node_frame_),
      local_point_(other.LocalPoint()),
      inner_url_element_(other.URLElement()),
      scrollbar_(other.GetScrollbar()),
      is_over_embedded_content_view_(other.IsOverEmbeddedContentView()),
      canvas_region_id_(other.CanvasRegionId()) {
  // Only copy the NodeSet in case of list hit test.
  list_based_test_result_ = other.list_based_test_result_
                                ? new NodeSet(*other.list_based_test_result_)
                                : nullptr;
}

HitTestResult::~HitTestResult() = default;

HitTestResult& HitTestResult::operator=(const HitTestResult& other) {
  hit_test_request_ = other.hit_test_request_;
  PopulateFromCachedResult(other);

  return *this;
}

bool HitTestResult::EqualForCacheability(const HitTestResult& other) const {
  return hit_test_request_.EqualForCacheability(other.hit_test_request_) &&
         inner_node_ == other.InnerNode() &&
         inner_possibly_pseudo_node_ == other.InnerPossiblyPseudoNode() &&
         point_in_inner_node_frame_ == other.point_in_inner_node_frame_ &&
         local_point_ == other.LocalPoint() &&
         inner_url_element_ == other.URLElement() &&
         scrollbar_ == other.GetScrollbar() &&
         is_over_embedded_content_view_ == other.IsOverEmbeddedContentView();
}

void HitTestResult::CacheValues(const HitTestResult& other) {
  hit_test_request_ =
      other.hit_test_request_.GetType() & ~HitTestRequest::kAvoidCache;
}

void HitTestResult::PopulateFromCachedResult(const HitTestResult& other) {
  inner_node_ = other.InnerNode();
  inner_possibly_pseudo_node_ = other.InnerPossiblyPseudoNode();
  point_in_inner_node_frame_ = other.point_in_inner_node_frame_;
  local_point_ = other.LocalPoint();
  inner_url_element_ = other.URLElement();
  scrollbar_ = other.GetScrollbar();
  is_over_embedded_content_view_ = other.IsOverEmbeddedContentView();
  cacheable_ = other.cacheable_;
  canvas_region_id_ = other.CanvasRegionId();

  // Only copy the NodeSet in case of list hit test.
  list_based_test_result_ = other.list_based_test_result_
                                ? new NodeSet(*other.list_based_test_result_)
                                : nullptr;
}

void HitTestResult::Trace(blink::Visitor* visitor) {
  visitor->Trace(inner_node_);
  visitor->Trace(inner_possibly_pseudo_node_);
  visitor->Trace(inner_url_element_);
  visitor->Trace(scrollbar_);
  visitor->Trace(list_based_test_result_);
}

PositionWithAffinity HitTestResult::GetPosition() const {
  if (!inner_possibly_pseudo_node_)
    return PositionWithAffinity();
  LayoutObject* layout_object = GetLayoutObject();
  if (!layout_object)
    return PositionWithAffinity();
  if (inner_possibly_pseudo_node_->IsPseudoElement() &&
      inner_possibly_pseudo_node_->GetPseudoId() == kPseudoIdBefore) {
    return PositionWithAffinity(MostForwardCaretPosition(
        Position(inner_node_, PositionAnchorType::kBeforeChildren)));
  }
  return layout_object->PositionForPoint(LocalPoint());
}

LayoutObject* HitTestResult::GetLayoutObject() const {
  return inner_node_ ? inner_node_->GetLayoutObject() : nullptr;
}

void HitTestResult::SetToShadowHostIfInRestrictedShadowRoot() {
  Node* node = InnerNode();
  if (!node)
    return;

  ShadowRoot* containing_shadow_root = node->ContainingShadowRoot();
  Element* shadow_host = nullptr;

  ASSERT(false); // BKTODO:
#if 0
  // Consider a closed shadow tree of SVG's <use> element as a special
  // case so that a toolip title in the shadow tree works.
  while (containing_shadow_root &&
         (containing_shadow_root->IsUserAgent() ||
          IsSVGUseElement(containing_shadow_root->host()))) {
    shadow_host = &containing_shadow_root->host();
    containing_shadow_root = shadow_host->ContainingShadowRoot();
    SetInnerNode(node->OwnerShadowHost());
  }
#endif

  if (shadow_host)
    SetInnerNode(shadow_host);
}

HTMLAreaElement* HitTestResult::ImageAreaForImage() const {
  DCHECK(inner_node_);
  ASSERT(false); // BKTODO:
  return nullptr;
#if 0
  HTMLImageElement* image_element = ToHTMLImageElementOrNull(inner_node_);
  if (!image_element && inner_node_->IsInShadowTree()) {
    if (inner_node_->ContainingShadowRoot()->IsUserAgent()) {
      image_element = ToHTMLImageElementOrNull(inner_node_->OwnerShadowHost());
    }
  }

  if (!image_element || !image_element->GetLayoutObject() ||
      !image_element->GetLayoutObject()->IsBox())
    return nullptr;

  HTMLMapElement* map = image_element->GetTreeScope().GetImageMap(
      image_element->FastGetAttribute(usemapAttr));
  if (!map)
    return nullptr;

  return map->AreaForPoint(LocalPoint(), image_element->GetLayoutObject());
#endif
}

void HitTestResult::SetInnerNode(Node* n) {
  inner_possibly_pseudo_node_ = n;
  if (!n) {
    inner_node_ = n;
    return;
  }
  if (n->IsPseudoElement())
    n = ToPseudoElement(n)->InnerNodeForHitTesting();
  inner_node_ = n;
  if (HTMLAreaElement* area = ImageAreaForImage()) {
    ASSERT(false); // BKTODO:
#if 0
    inner_node_ = area;
    inner_possibly_pseudo_node_ = area;
#endif
  }
}

void HitTestResult::SetURLElement(Element* n) {
  inner_url_element_ = n;
}

void HitTestResult::SetScrollbar(Scrollbar* s) {
  scrollbar_ = s;
}

LocalFrame* HitTestResult::InnerNodeFrame() const {
  if (inner_node_)
    return inner_node_->GetDocument().GetFrame();
  return nullptr;
}

bool HitTestResult::IsSelected(const HitTestLocation& location) const {
  if (!inner_node_)
    return false;

  if (LocalFrame* frame = inner_node_->GetDocument().GetFrame())
    return frame->Selection().Contains(location.Point());
  return false;
}

String HitTestResult::Title(TextDirection& dir) const {
  dir = TextDirection::kLtr;
  ASSERT(false); // BKTODO:
#if 0
  // Find the title in the nearest enclosing DOM node.
  // For <area> tags in image maps, walk the tree for the <area>, not the <img>
  // using it.
  if (inner_node_.Get())
    inner_node_->UpdateDistributionForFlatTreeTraversal();
#endif
  for (Node* title_node = inner_node_.Get(); title_node;
       title_node = FlatTreeTraversal::Parent(*title_node)) {
    if (title_node->IsElementNode()) {
      ASSERT(false); // BKTODO:
#if 0
      String title = ToElement(title_node)->title();
      if (!title.IsNull()) {
        if (LayoutObject* layout_object = title_node->GetLayoutObject())
          dir = layout_object->StyleRef().Direction();
        return title;
      }
#endif
    }
  }
  return String();
}

const AtomicString& HitTestResult::AltDisplayString() const {
  Node* inner_node_or_image_map_image = InnerNodeOrImageMapImage();
  if (!inner_node_or_image_map_image)
    return g_null_atom;

  ASSERT(false); // BKTODO:
#if 0
  if (auto* image = ToHTMLImageElementOrNull(*inner_node_or_image_map_image))
    return image->getAttribute(altAttr);

  if (auto* input = ToHTMLInputElementOrNull(*inner_node_or_image_map_image))
    return input->Alt();
#endif

  return g_null_atom;
}

Image* HitTestResult::GetImage() const {
  Node* inner_node_or_image_map_image = InnerNodeOrImageMapImage();
  if (!inner_node_or_image_map_image)
    return nullptr;

  LayoutObject* layout_object =
      inner_node_or_image_map_image->GetLayoutObject();
  if (layout_object && layout_object->IsImage()) {
    LayoutImage* image = ToLayoutImage(layout_object);
    if (image->CachedImage() && !image->CachedImage()->ErrorOccurred())
      return image->CachedImage()->GetImage();
  }

  return nullptr;
}

IntRect HitTestResult::ImageRect() const {
  if (!GetImage())
    return IntRect();
  ASSERT(false); // BKTODO:
  return IntRect();
#if 0
  return InnerNodeOrImageMapImage()
      ->GetLayoutBox()
      ->AbsoluteContentQuad()
      .EnclosingBoundingBox();
#endif
}

GURL HitTestResult::AbsoluteImageURL() const {
  Node* inner_node_or_image_map_image = InnerNodeOrImageMapImage();
  if (!inner_node_or_image_map_image)
    return GURL();

  AtomicString url_string;
  ASSERT(false); // BKTODO:
#if 0
  // Always return a url for image elements and input elements with type=image,
  // even if they don't have a LayoutImage (e.g. because the image didn't load
  // and we are using an alt container). For other elements we don't create alt
  // containers so ensure they contain a loaded image.
  if (IsHTMLImageElement(*inner_node_or_image_map_image) ||
      (IsHTMLInputElement(*inner_node_or_image_map_image) &&
       ToHTMLInputElement(inner_node_or_image_map_image)->type() ==
           InputTypeNames::image))
    url_string = ToElement(*inner_node_or_image_map_image).ImageSourceURL();
  else if ((inner_node_or_image_map_image->GetLayoutObject() &&
            inner_node_or_image_map_image->GetLayoutObject()->IsImage()) &&
           (IsHTMLEmbedElement(*inner_node_or_image_map_image) ||
            IsHTMLObjectElement(*inner_node_or_image_map_image) ||
            IsSVGImageElement(*inner_node_or_image_map_image)))
    url_string = ToElement(*inner_node_or_image_map_image).ImageSourceURL();
#endif
  if (url_string.IsEmpty())
    return GURL();

  return inner_node_or_image_map_image->GetDocument().CompleteURL(
      StripLeadingAndTrailingHTMLSpaces(url_string));
}

GURL HitTestResult::AbsoluteMediaURL() const {
  ASSERT(false); // BKTODO:
#if 0
  if (HTMLMediaElement* media_elt = MediaElement())
    return media_elt->currentSrc();
#endif
  return GURL();
}

MediaStreamDescriptor* HitTestResult::GetMediaStreamDescriptor() const {
  ASSERT(false); // BKTODO:
#if 0
  if (HTMLMediaElement* media_elt = MediaElement())
    return media_elt->GetSrcObject();
#endif
  return nullptr;
}

HTMLMediaElement* HitTestResult::MediaElement() const {
  if (!inner_node_)
    return nullptr;

  ASSERT(false); // BKTODO:
#if 0
  if (!(inner_node_->GetLayoutObject() &&
        inner_node_->GetLayoutObject()->IsMedia()))
    return nullptr;

  if (IsHTMLMediaElement(*inner_node_))
    return ToHTMLMediaElement(inner_node_);
#endif
  return nullptr;
}

GURL HitTestResult::AbsoluteLinkURL() const {
  if (!inner_url_element_)
    return GURL();
  ASSERT(false); // BKTODO:
  return GURL();
#if 0
  return inner_url_element_->HrefURL();
#endif
}

bool HitTestResult::IsLiveLink() const {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return inner_url_element_ && inner_url_element_->IsLiveLink();
#endif
}

bool HitTestResult::IsOverLink() const {
  return inner_url_element_ && inner_url_element_->IsLink();
}

String HitTestResult::TextContent() const {
  if (!inner_url_element_)
    return String();
  return inner_url_element_->textContent();
}

// FIXME: This function needs a better name and may belong in a different class.
// It's not really isContentEditable(); it's more like needsEditingContextMenu.
// In many ways, this function would make more sense in the ContextMenu class,
// except that WebElementDictionary hooks into it. Anyway, we should architect
// this better.
bool HitTestResult::IsContentEditable() const {
  if (!inner_node_)
    return false;

  ASSERT(false); // BKTODO:
#if 0
  if (auto* textarea = ToHTMLTextAreaElementOrNull(*inner_node_))
    return !textarea->IsDisabledOrReadOnly();

  if (auto* input = ToHTMLInputElementOrNull(*inner_node_))
    return !input->IsDisabledOrReadOnly() && input->IsTextField();
#endif

  return HasEditableStyle(*inner_node_);
}

ListBasedHitTestBehavior HitTestResult::AddNodeToListBasedTestResult(
    Node* node,
    const HitTestLocation& location,
    const LayoutRect& rect) {
  // If not a list-based test, stop testing because the hit has been found.
  if (!GetHitTestRequest().ListBased())
    return kStopHitTesting;

  if (!node)
    return kContinueHitTesting;

  ASSERT(false); // BKTODO: MutableListBasedTestResult().insert(node);

  if (GetHitTestRequest().PenetratingList())
    return kContinueHitTesting;

  return rect.Contains(location.BoundingBox()) ? kStopHitTesting
                                               : kContinueHitTesting;
}

ListBasedHitTestBehavior HitTestResult::AddNodeToListBasedTestResult(
    Node* node,
    const HitTestLocation& location,
    const Region& region) {
  // If not a list-based test, stop testing because the hit has been found.
  if (!GetHitTestRequest().ListBased())
    return kStopHitTesting;

  if (!node)
    return kContinueHitTesting;

  ASSERT(false); // BKTODO: MutableListBasedTestResult().insert(node);

  if (GetHitTestRequest().PenetratingList())
    return kContinueHitTesting;

  return region.Contains(location.EnclosingIntRect()) ? kStopHitTesting
                                                      : kContinueHitTesting;
}

void HitTestResult::Append(const HitTestResult& other) {
  DCHECK(GetHitTestRequest().ListBased());

  if (!scrollbar_ && other.GetScrollbar()) {
    SetScrollbar(other.GetScrollbar());
  }

  if (!inner_node_ && other.InnerNode()) {
    inner_node_ = other.InnerNode();
    inner_possibly_pseudo_node_ = other.InnerPossiblyPseudoNode();
    local_point_ = other.LocalPoint();
    point_in_inner_node_frame_ = other.point_in_inner_node_frame_;
    inner_url_element_ = other.URLElement();
    is_over_embedded_content_view_ = other.IsOverEmbeddedContentView();
    canvas_region_id_ = other.CanvasRegionId();
  }

  if (other.list_based_test_result_) {
    NodeSet& set = MutableListBasedTestResult();
    ASSERT(false); // BKTODO:
#if 0
    for (NodeSet::const_iterator it = other.list_based_test_result_->begin(),
                                 last = other.list_based_test_result_->end();
         it != last; ++it)
      set.insert(it->Get());
#endif
  }
}

const HitTestResult::NodeSet& HitTestResult::ListBasedTestResult() const {
  if (!list_based_test_result_)
    list_based_test_result_ = new NodeSet;
  return *list_based_test_result_;
}

HitTestResult::NodeSet& HitTestResult::MutableListBasedTestResult() {
  if (!list_based_test_result_)
    list_based_test_result_ = new NodeSet;
  return *list_based_test_result_;
}

HitTestLocation HitTestResult::ResolveRectBasedTest(
    Node* resolved_inner_node,
    const LayoutPoint& resolved_point_in_main_frame) {
  point_in_inner_node_frame_ = resolved_point_in_main_frame;
  inner_node_ = nullptr;
  inner_possibly_pseudo_node_ = nullptr;
  list_based_test_result_ = nullptr;

  // Update the HitTestResult as if the supplied node had been hit in normal
  // point-based hit-test.
  // Note that we don't know the local point after a rect-based hit-test, but we
  // never use it so shouldn't bother with the cost of computing it.
  DCHECK(resolved_inner_node);
  if (auto* layout_object = resolved_inner_node->GetLayoutObject())
    layout_object->UpdateHitTestResult(*this, LayoutPoint());

  return HitTestLocation(resolved_point_in_main_frame);
}

Element* HitTestResult::InnerElement() const {
  if (!inner_node_)
    return nullptr;
  if (inner_node_->IsElementNode())
    return ToElement(inner_node_);
  return FlatTreeTraversal::ParentElement(*inner_node_);
}

Node* HitTestResult::InnerNodeOrImageMapImage() const {
  if (!inner_node_)
    return nullptr;

  ASSERT(false); // BKTODO:
  return nullptr;
#if 0
  HTMLImageElement* image_map_image_element = nullptr;
  if (auto* area = ToHTMLAreaElementOrNull(inner_node_))
    image_map_image_element = area->ImageElement();
  else if (auto* map = ToHTMLMapElementOrNull(inner_node_))
    image_map_image_element = map->ImageElement();

  if (!image_map_image_element)
    return inner_node_.Get();

  return image_map_image_element;
#endif
}

}  // namespace blink
