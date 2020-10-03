// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: ng_line_box_fragment_builder.cc
// Description: NGLineBoxFragmentBuilder Class
//      Author: Ziming Li
//     Created: 2020-10-03
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/layout/ng/inline/ng_line_box_fragment_builder.h"

#include "third_party/blink/renderer/core/layout/ng/exclusions/ng_exclusion_space.h"
#include "third_party/blink/renderer/core/layout/ng/inline/ng_inline_break_token.h"
#include "third_party/blink/renderer/core/layout/ng/inline/ng_inline_node.h"
#include "third_party/blink/renderer/core/layout/ng/inline/ng_physical_line_box_fragment.h"
#include "third_party/blink/renderer/core/layout/ng/ng_fragment.h"
#include "third_party/blink/renderer/core/layout/ng/ng_layout_result.h"
#include "third_party/blink/renderer/core/layout/ng/ng_positioned_float.h"

namespace blink {

NGLineBoxFragmentBuilder::NGLineBoxFragmentBuilder(
    NGInlineNode node,
    scoped_refptr<const ComputedStyle> style,
    WritingMode writing_mode,
    TextDirection)
    : NGContainerFragmentBuilder(style, writing_mode, TextDirection::kLtr),
      node_(node),
      base_direction_(TextDirection::kLtr) {}

NGLineBoxFragmentBuilder::~NGLineBoxFragmentBuilder() = default;

void NGLineBoxFragmentBuilder::Reset() {
  children_.clear();
  offsets_.clear();
  metrics_ = NGLineHeightMetrics();
  size_.inline_size = LayoutUnit();
}

LayoutUnit NGLineBoxFragmentBuilder::LineHeight() const {
  return metrics_.LineHeight().ClampNegativeToZero();
}

const NGPhysicalFragment* NGLineBoxFragmentBuilder::Child::PhysicalFragment()
    const {
  return layout_result ? layout_result->PhysicalFragment().get()
                       : fragment.get();
}

NGLineBoxFragmentBuilder::Child*
NGLineBoxFragmentBuilder::ChildList::FirstInFlowChild() {
  for (auto& child : *this) {
    if (child.HasInFlowFragment())
      return &child;
  }
  return nullptr;
}

NGLineBoxFragmentBuilder::Child*
NGLineBoxFragmentBuilder::ChildList::LastInFlowChild() {
  for (auto it = rbegin(); it != rend(); it++) {
    auto& child = *it;
    if (child.HasInFlowFragment())
      return &child;
  }
  return nullptr;
}

#if 0 // BKTODO:
void NGLineBoxFragmentBuilder::ChildList::InsertChild(
    unsigned index,
    scoped_refptr<NGLayoutResult> layout_result,
    const NGLogicalOffset& offset,
    LayoutUnit inline_size,
    UBiDiLevel bidi_level) {
  children_.insert(
      index, Child{std::move(layout_result), offset, inline_size, bidi_level});
}
#endif

void NGLineBoxFragmentBuilder::ChildList::MoveInInlineDirection(
    LayoutUnit delta,
    unsigned start,
    unsigned end) {
  for (unsigned index = start; index < end; index++)
    children_[index].offset.inline_offset += delta;
}

void NGLineBoxFragmentBuilder::ChildList::MoveInBlockDirection(
    LayoutUnit delta) {
  for (auto& child : children_)
    child.offset.block_offset += delta;
}

void NGLineBoxFragmentBuilder::ChildList::MoveInBlockDirection(LayoutUnit delta,
                                                               unsigned start,
                                                               unsigned end) {
  for (unsigned index = start; index < end; index++)
    children_[index].offset.block_offset += delta;
}

void NGLineBoxFragmentBuilder::SetMetrics(const NGLineHeightMetrics& metrics) {
  metrics_ = metrics;
}

void NGLineBoxFragmentBuilder::SetBaseDirection(TextDirection direction) {
  base_direction_ = direction;
}

void NGLineBoxFragmentBuilder::SwapPositionedFloats(
    Vector<NGPositionedFloat>* positioned_floats) {
  positioned_floats_.swap(*positioned_floats);
}

void NGLineBoxFragmentBuilder::SetBreakToken(
    scoped_refptr<NGInlineBreakToken> break_token) {
  break_token_ = std::move(break_token);
}

void NGLineBoxFragmentBuilder::AddChildren(ChildList& children) {
  offsets_.ReserveCapacity(children.size());
  children_.ReserveCapacity(children.size());

  for (auto& child : children) {
    if (child.layout_result) {
      DCHECK(!child.fragment);
      AddChild(*child.layout_result, child.offset);
      ASSERT(false); // BKTODO: child.layout_result.reset();
    } else if (child.fragment) {
      AddChild(std::move(child.fragment), child.offset);
      DCHECK(!child.fragment);
    }
  }
}

scoped_refptr<NGLayoutResult> NGLineBoxFragmentBuilder::ToLineBoxFragment() {
  DCHECK_EQ(offsets_.size(), children_.size());

  WritingMode line_writing_mode(ToLineWritingMode(GetWritingMode()));
  NGPhysicalSize physical_size = Size().ConvertToPhysical(line_writing_mode);

  DCHECK_EQ(children_.size(), offsets_.size());
  for (size_t i = 0; i < children_.size(); i++) {
    auto& child = children_[i];
    child.offset_ = offsets_[i].ConvertToPhysical(
        line_writing_mode, Direction(), physical_size, child->Size());
  }

  // Because this vector will be long-lived, make sure to not waaste space.
  // (We reserve an initial capacity when adding the first child)
  if (children_.size())
    children_.ShrinkToReasonableCapacity();

  scoped_refptr<const NGPhysicalLineBoxFragment> fragment =
      base::AdoptRef(new NGPhysicalLineBoxFragment(
          Style(), style_variant_, physical_size, children_, metrics_,
          base_direction_,
          break_token_ ? std::move(break_token_)
                       : NGInlineBreakToken::Create(node_)));

  return base::AdoptRef(new NGLayoutResult(
      std::move(fragment), std::move(oof_positioned_descendants_),
      std::move(positioned_floats_), unpositioned_list_marker_,
      std::move(exclusion_space_), bfc_line_offset_, bfc_block_offset_,
      end_margin_strut_,
      /* intrinsic_block_size */ LayoutUnit(),
      /* minimal_space_shortage */ LayoutUnit::Max(), EBreakBetween::kAuto,
      EBreakBetween::kAuto, /* has_forced_break */ false, is_pushed_by_floats_,
      adjoining_floats_, NGLayoutResult::kSuccess));
}

}  // namespace blink
