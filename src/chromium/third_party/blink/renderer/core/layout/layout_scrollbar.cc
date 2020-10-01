// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: layout_scrollbar.cc
// Description: LayoutScrollbar Class
//      Author: Ziming Li
//     Created: 2020-10-01
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2008, 2009 Apple Inc. All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "third_party/blink/renderer/core/layout/layout_scrollbar.h"

#include "third_party/blink/renderer/core/css/pseudo_style_request.h"
#include "third_party/blink/renderer/core/frame/local_frame_view.h"
#if 0 // BKTODO:
#include "third_party/blink/renderer/core/layout/layout_embedded_content.h"
#include "third_party/blink/renderer/core/layout/layout_scrollbar_part.h"
#include "third_party/blink/renderer/core/layout/layout_scrollbar_theme.h"
#endif
#include "third_party/blink/renderer/core/layout/layout_view.h"
#include "third_party/blink/renderer/core/paint/object_paint_invalidator.h"
#include "third_party/blink/renderer/platform/graphics/graphics_context.h"

namespace blink {

Scrollbar* LayoutScrollbar::CreateCustomScrollbar(
    ScrollableArea* scrollable_area,
    ScrollbarOrientation orientation,
    Element* style_source) {
  return new LayoutScrollbar(scrollable_area, orientation, style_source);
}

LayoutScrollbar::LayoutScrollbar(ScrollableArea* scrollable_area,
                                 ScrollbarOrientation orientation,
                                 Element* style_source)
    : Scrollbar(scrollable_area,
                orientation,
                kRegularScrollbar,
                nullptr,
#if 0 // BKTODO:
                LayoutScrollbarTheme::GetLayoutScrollbarTheme()),
#else
                nullptr),
#endif
      style_source_(style_source) {
  DCHECK(style_source);

  // FIXME: We need to do this because LayoutScrollbar::styleChanged is called
  // as soon as the scrollbar is created.

  // Update the scrollbar size.
  IntRect rect(0, 0, 0, 0);
  UpdateScrollbarPart(kScrollbarBGPart);
  ASSERT(false); // BKTODO:
#if 0
  if (LayoutScrollbarPart* part = parts_.at(kScrollbarBGPart)) {
    part->UpdateLayout();
    rect.SetSize(FlooredIntSize(part->Size()));
  } else if (Orientation() == kHorizontalScrollbar) {
    rect.SetWidth(Width());
  } else {
    rect.SetHeight(Height());
  }
#endif

  SetFrameRect(rect);
}

LayoutScrollbar::~LayoutScrollbar() {
  ASSERT(false); // BKTODO:
#if 0
  if (parts_.IsEmpty())
    return;
#endif

  // When a scrollbar is detached from its parent (causing all parts removal)
  // and ready to be destroyed, its destruction can be delayed because of
  // RefPtr maintained in other classes such as EventHandler
  // (m_lastScrollbarUnderMouse).
  // Meanwhile, we can have a call to updateScrollbarPart which recreates the
  // scrollbar part. So, we need to destroy these parts since we don't want them
  // to call on a destroyed scrollbar. See webkit bug 68009.
  UpdateScrollbarParts(true);
}

int LayoutScrollbar::HypotheticalScrollbarThickness(
    ScrollbarOrientation orientation,
    const LayoutBox& enclosing_box,
    const LayoutObject& style_source) {
  scoped_refptr<const ComputedStyle> part_style =
      style_source.GetUncachedPseudoStyle(
          PseudoStyleRequest(kPseudoIdScrollbar, nullptr, kScrollbarBGPart),
          style_source.Style());
  ASSERT(false); // BKTODO:
  return 0;
#if 0
  if (orientation == kHorizontalScrollbar) {
    return LayoutScrollbarPart::ComputeScrollbarHeight(
        enclosing_box.ClientHeight().ToInt(), part_style.get());
  }
  return LayoutScrollbarPart::ComputeScrollbarWidth(
      enclosing_box.ClientWidth().ToInt(), part_style.get());
#endif
}

void LayoutScrollbar::DisconnectFromScrollableArea() {
  UpdateScrollbarParts(true);
  Scrollbar::DisconnectFromScrollableArea();
}

void LayoutScrollbar::SetEnabled(bool e) {
  bool was_enabled = Enabled();
  Scrollbar::SetEnabled(e);
  if (was_enabled != e)
    UpdateScrollbarParts();
}

void LayoutScrollbar::StyleChanged() {
  UpdateScrollbarParts();
}

void LayoutScrollbar::SetHoveredPart(ScrollbarPart part) {
  if (part == hovered_part_)
    return;

  ScrollbarPart old_part = hovered_part_;
  hovered_part_ = part;

  UpdateScrollbarPart(old_part);
  UpdateScrollbarPart(hovered_part_);

  UpdateScrollbarPart(kScrollbarBGPart);
  UpdateScrollbarPart(kTrackBGPart);
}

void LayoutScrollbar::SetPressedPart(ScrollbarPart part) {
  ScrollbarPart old_part = pressed_part_;
  Scrollbar::SetPressedPart(part);

  UpdateScrollbarPart(old_part);
  UpdateScrollbarPart(part);

  UpdateScrollbarPart(kScrollbarBGPart);
  UpdateScrollbarPart(kTrackBGPart);
}

scoped_refptr<ComputedStyle> LayoutScrollbar::GetScrollbarPseudoStyle(
    ScrollbarPart part_type,
    PseudoId pseudo_id) {
  if (!style_source_->GetLayoutObject())
    return nullptr;
  ASSERT(false); // BKTODO:
  return nullptr;
#if 0
  return style_source_->StyleForPseudoElement(
      PseudoStyleRequest(pseudo_id, this, part_type),
      style_source_->GetLayoutObject()->Style());
#endif
}

void LayoutScrollbar::UpdateScrollbarParts(bool destroy) {
  UpdateScrollbarPart(kScrollbarBGPart, destroy);
  UpdateScrollbarPart(kBackButtonStartPart, destroy);
  UpdateScrollbarPart(kForwardButtonStartPart, destroy);
  UpdateScrollbarPart(kBackTrackPart, destroy);
  UpdateScrollbarPart(kThumbPart, destroy);
  UpdateScrollbarPart(kForwardTrackPart, destroy);
  UpdateScrollbarPart(kBackButtonEndPart, destroy);
  UpdateScrollbarPart(kForwardButtonEndPart, destroy);
  UpdateScrollbarPart(kTrackBGPart, destroy);

  if (destroy)
    return;

  // See if the scrollbar's thickness changed.  If so, we need to mark our
  // owning object as needing a layout.
  bool is_horizontal = Orientation() == kHorizontalScrollbar;
  int old_thickness = is_horizontal ? Height() : Width();
  int new_thickness = 0;
  ASSERT(false); // BKTODO:
#if 0
  LayoutScrollbarPart* part = parts_.at(kScrollbarBGPart);
  if (part) {
    part->UpdateLayout();
    new_thickness =
        (is_horizontal ? part->Size().Height() : part->Size().Width()).ToInt();
  }
#endif

  if (new_thickness != old_thickness) {
    SetFrameRect(
        IntRect(Location(), IntSize(is_horizontal ? Width() : new_thickness,
                                    is_horizontal ? new_thickness : Height())));
    if (LayoutBox* box = GetScrollableArea()->GetLayoutBox()) {
      if (box->IsLayoutBlock())
        ToLayoutBlock(box)->NotifyScrollbarThicknessChanged();
      box->SetChildNeedsLayout();
      if (scrollable_area_)
        scrollable_area_->SetScrollCornerNeedsPaintInvalidation();
    }
  }
}

static PseudoId PseudoForScrollbarPart(ScrollbarPart part) {
  switch (part) {
    case kBackButtonStartPart:
    case kForwardButtonStartPart:
    case kBackButtonEndPart:
    case kForwardButtonEndPart:
      return kPseudoIdScrollbarButton;
    case kBackTrackPart:
    case kForwardTrackPart:
      return kPseudoIdScrollbarTrackPiece;
    case kThumbPart:
      return kPseudoIdScrollbarThumb;
    case kTrackBGPart:
      return kPseudoIdScrollbarTrack;
    case kScrollbarBGPart:
      return kPseudoIdScrollbar;
    case kNoPart:
    case kAllParts:
      break;
  }
  NOTREACHED();
  return kPseudoIdScrollbar;
}

void LayoutScrollbar::UpdateScrollbarPart(ScrollbarPart part_type,
                                          bool destroy) {
  if (part_type == kNoPart)
    return;

  scoped_refptr<ComputedStyle> part_style =
      !destroy ? GetScrollbarPseudoStyle(part_type,
                                         PseudoForScrollbarPart(part_type))
               : scoped_refptr<ComputedStyle>(nullptr);

  bool need_layout_object =
      !destroy && part_style && part_style->Display() != EDisplay::kNone;

  if (need_layout_object && part_style->Display() != EDisplay::kBlock) {
    ASSERT(false); // BKTODO:
#if 0
    // See if we are a button that should not be visible according to OS
    // settings.
    WebScrollbarButtonsPlacement buttons_placement =
        GetTheme().ButtonsPlacement();
    switch (part_type) {
      case kBackButtonStartPart:
        need_layout_object =
            (buttons_placement == kWebScrollbarButtonsPlacementSingle ||
             buttons_placement == kWebScrollbarButtonsPlacementDoubleStart ||
             buttons_placement == kWebScrollbarButtonsPlacementDoubleBoth);
        break;
      case kForwardButtonStartPart:
        need_layout_object =
            (buttons_placement == kWebScrollbarButtonsPlacementDoubleStart ||
             buttons_placement == kWebScrollbarButtonsPlacementDoubleBoth);
        break;
      case kBackButtonEndPart:
        need_layout_object =
            (buttons_placement == kWebScrollbarButtonsPlacementDoubleEnd ||
             buttons_placement == kWebScrollbarButtonsPlacementDoubleBoth);
        break;
      case kForwardButtonEndPart:
        need_layout_object =
            (buttons_placement == kWebScrollbarButtonsPlacementSingle ||
             buttons_placement == kWebScrollbarButtonsPlacementDoubleEnd ||
             buttons_placement == kWebScrollbarButtonsPlacementDoubleBoth);
        break;
      default:
        break;
    }
#endif
  }

  ASSERT(false); // BKTODO:
#if 0
  LayoutScrollbarPart* part_layout_object = parts_.at(part_type);
  if (!part_layout_object && need_layout_object && scrollable_area_) {
    part_layout_object = LayoutScrollbarPart::CreateAnonymous(
        &style_source_->GetDocument(), scrollable_area_, this, part_type);
    parts_.Set(part_type, part_layout_object);
    SetNeedsPaintInvalidation(part_type);
  } else if (part_layout_object && !need_layout_object) {
    parts_.erase(part_type);
    part_layout_object->Destroy();
    part_layout_object = nullptr;
    if (!destroy)
      SetNeedsPaintInvalidation(part_type);
  }

  if (part_layout_object)
    part_layout_object->SetStyleWithWritingModeOfParent(std::move(part_style));
#endif
}

IntRect LayoutScrollbar::ButtonRect(ScrollbarPart part_type) const {
  ASSERT(false); // BKTODO:
  return IntRect();
#if 0
  LayoutScrollbarPart* part_layout_object = parts_.at(part_type);
  if (!part_layout_object)
    return IntRect();

  part_layout_object->UpdateLayout();

  bool is_horizontal = Orientation() == kHorizontalScrollbar;
  if (part_type == kBackButtonStartPart)
    return IntRect(
        Location(),
        IntSize(
            is_horizontal ? part_layout_object->PixelSnappedWidth() : Width(),
            is_horizontal ? Height()
                          : part_layout_object->PixelSnappedHeight()));
  if (part_type == kForwardButtonEndPart) {
    return IntRect(
        is_horizontal ? X() + Width() - part_layout_object->PixelSnappedWidth()
                      : X(),
        is_horizontal
            ? Y()
            : Y() + Height() - part_layout_object->PixelSnappedHeight(),
        is_horizontal ? part_layout_object->PixelSnappedWidth() : Width(),
        is_horizontal ? Height() : part_layout_object->PixelSnappedHeight());
  }

  if (part_type == kForwardButtonStartPart) {
    IntRect previous_button = ButtonRect(kBackButtonStartPart);
    return IntRect(
        is_horizontal ? X() + previous_button.Width() : X(),
        is_horizontal ? Y() : Y() + previous_button.Height(),
        is_horizontal ? part_layout_object->PixelSnappedWidth() : Width(),
        is_horizontal ? Height() : part_layout_object->PixelSnappedHeight());
  }

  IntRect following_button = ButtonRect(kForwardButtonEndPart);
  return IntRect(
      is_horizontal ? X() + Width() - following_button.Width() -
                          part_layout_object->PixelSnappedWidth()
                    : X(),
      is_horizontal ? Y()
                    : Y() + Height() - following_button.Height() -
                          part_layout_object->PixelSnappedHeight(),
      is_horizontal ? part_layout_object->PixelSnappedWidth() : Width(),
      is_horizontal ? Height() : part_layout_object->PixelSnappedHeight());
#endif
}

IntRect LayoutScrollbar::TrackRect(int start_length, int end_length) const {
  ASSERT(false); // BKTODO:
  return IntRect();
#if 0
  LayoutScrollbarPart* part = parts_.at(kTrackBGPart);
  if (part)
    part->UpdateLayout();

  if (Orientation() == kHorizontalScrollbar) {
    int margin_left = part ? part->MarginLeft().ToInt() : 0;
    int margin_right = part ? part->MarginRight().ToInt() : 0;
    start_length += margin_left;
    end_length += margin_right;
    int total_length = start_length + end_length;
    return IntRect(X() + start_length, Y(), Width() - total_length, Height());
  }

  int margin_top = part ? part->MarginTop().ToInt() : 0;
  int margin_bottom = part ? part->MarginBottom().ToInt() : 0;
  start_length += margin_top;
  end_length += margin_bottom;
  int total_length = start_length + end_length;

  return IntRect(X(), Y() + start_length, Width(), Height() - total_length);
#endif
}

IntRect LayoutScrollbar::TrackPieceRectWithMargins(
    ScrollbarPart part_type,
    const IntRect& old_rect) const {
  ASSERT(false); // BKTODO:
#if 0
  LayoutScrollbarPart* part_layout_object = parts_.at(part_type);
  if (!part_layout_object)
    return old_rect;

  part_layout_object->UpdateLayout();
#endif

  IntRect rect = old_rect;
  ASSERT(false); // BKTODO:
#if 0
  if (Orientation() == kHorizontalScrollbar) {
    rect.SetX((rect.X() + part_layout_object->MarginLeft()).ToInt());
    rect.SetWidth((rect.Width() - part_layout_object->MarginWidth()).ToInt());
  } else {
    rect.SetY((rect.Y() + part_layout_object->MarginTop()).ToInt());
    rect.SetHeight(
        (rect.Height() - part_layout_object->MarginHeight()).ToInt());
  }
#endif
  return rect;
}

int LayoutScrollbar::MinimumThumbLength() const {
  ASSERT(false); // BKTODO:
  return 0;
#if 0
  LayoutScrollbarPart* part_layout_object = parts_.at(kThumbPart);
  if (!part_layout_object)
    return 0;
  part_layout_object->UpdateLayout();
  return (Orientation() == kHorizontalScrollbar
              ? part_layout_object->Size().Width()
              : part_layout_object->Size().Height())
      .ToInt();
#endif
}

void LayoutScrollbar::InvalidateDisplayItemClientsOfScrollbarParts() {
  ASSERT(false); // BKTODO:
#if 0
  for (auto& part : parts_) {
    ObjectPaintInvalidator(*part.value)
        .InvalidateDisplayItemClientsIncludingNonCompositingDescendants(
            PaintInvalidationReason::kScrollControl);
  }
#endif
}

void LayoutScrollbar::SetVisualRect(const LayoutRect& rect) {
  Scrollbar::SetVisualRect(rect);
  ASSERT(false); // BKTODO:
#if 0
  for (auto& part : parts_)
    part.value->GetMutableForPainting().FirstFragment().SetVisualRect(rect);
#endif
}

}  // namespace blink
