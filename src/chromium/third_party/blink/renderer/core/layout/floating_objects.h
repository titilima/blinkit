// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: floating_objects.h
// Description: FloatingObject Class
//      Author: Ziming Li
//     Created: 2020-08-28
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2007 David Smith (catfish.man@gmail.com)
 * Copyright (C) 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011 Apple Inc.
 * All rights reserved.
 * Copyright (C) Research In Motion Limited 2010. All rights reserved.
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
 */

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_LAYOUT_FLOATING_OBJECTS_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_LAYOUT_FLOATING_OBJECTS_H_

#include <list>
#include <memory>
#include "base/macros.h"
#include "third_party/blink/renderer/platform/geometry/layout_rect.h"
#include "third_party/blink/renderer/platform/wtf/hash_map.h"
#include "third_party/blink/renderer/platform/wtf/pod_free_list_arena.h"
#include "third_party/blink/renderer/platform/wtf/pod_interval_tree.h"

namespace blink {

class LayoutBlockFlow;
class LayoutBox;
class RootInlineBox;

class FloatingObject {
  USING_FAST_MALLOC(FloatingObject);

 public:
#ifndef NDEBUG
  // Used by the PODIntervalTree for debugging the FloatingObject.
  template <class>
  friend struct WTF::ValueToString;
#endif

  // Note that Type uses bits so you can use FloatLeftRight as a mask to query
  // for both left and right.
  enum Type { kFloatLeft = 1, kFloatRight = 2, kFloatLeftRight = 3 };

  static std::unique_ptr<FloatingObject> Create(LayoutBox*);

  std::unique_ptr<FloatingObject> CopyToNewContainer(
      LayoutSize,
      bool should_paint = false,
      bool is_descendant = false) const;

  std::unique_ptr<FloatingObject> UnsafeClone() const;

  Type GetType() const { return static_cast<Type>(type_); }
  LayoutBox* GetLayoutObject() const { return layout_object_; }

  bool IsPlaced() const { return is_placed_; }
  void SetIsPlaced(bool placed = true) { is_placed_ = placed; }

  LayoutUnit X() const {
    DCHECK(IsPlaced());
    return frame_rect_.X();
  }
  LayoutUnit MaxX() const {
    DCHECK(IsPlaced());
    return frame_rect_.MaxX();
  }
  LayoutUnit Y() const {
    DCHECK(IsPlaced());
    return frame_rect_.Y();
  }
  LayoutUnit MaxY() const {
    DCHECK(IsPlaced());
    return frame_rect_.MaxY();
  }
  LayoutUnit Width() const { return frame_rect_.Width(); }
  LayoutUnit Height() const { return frame_rect_.Height(); }

  void SetX(LayoutUnit x) {
    DCHECK(!IsInPlacedTree());
    frame_rect_.SetX(x);
  }
  void SetY(LayoutUnit y) {
    DCHECK(!IsInPlacedTree());
    frame_rect_.SetY(y);
  }
  void SetWidth(LayoutUnit width) {
    DCHECK(!IsInPlacedTree());
    frame_rect_.SetWidth(width);
  }
  void SetHeight(LayoutUnit height) {
    DCHECK(!IsInPlacedTree());
    frame_rect_.SetHeight(height);
  }

  const LayoutRect& FrameRect() const {
    DCHECK(IsPlaced());
    return frame_rect_;
  }

  bool IsInPlacedTree() const { return is_in_placed_tree_; }
  void SetIsInPlacedTree(bool value) { is_in_placed_tree_ = value; }

  bool ShouldPaint() const { return should_paint_; }
  void SetShouldPaint(bool should_paint) { should_paint_ = should_paint; }
  bool IsDescendant() const { return is_descendant_; }
  void SetIsDescendant(bool is_descendant) { is_descendant_ = is_descendant; }
  bool IsLowestNonOverhangingFloatInChild() const {
    return is_lowest_non_overhanging_float_in_child_;
  }
  void SetIsLowestNonOverhangingFloatInChild(
      bool is_lowest_non_overhanging_float_in_child) {
    is_lowest_non_overhanging_float_in_child_ =
        is_lowest_non_overhanging_float_in_child;
  }

  // FIXME: Callers of these methods are dangerous and should be whitelisted
  // explicitly or removed.
  RootInlineBox* OriginatingLine() const { return originating_line_; }
  void SetOriginatingLine(RootInlineBox* line) { originating_line_ = line; }

 private:
  explicit FloatingObject(LayoutBox*);
  FloatingObject(LayoutBox*,
                 Type,
                 const LayoutRect&,
                 bool should_paint,
                 bool is_descendant,
                 bool is_lowest_non_overhanging_float_in_child);

  LayoutBox* layout_object_;
  RootInlineBox* originating_line_;
  LayoutRect frame_rect_;

  unsigned type_ : 2;  // Type (left or right aligned)
  unsigned should_paint_ : 1;
  unsigned is_descendant_ : 1;
  unsigned is_placed_ : 1;
  unsigned is_lowest_non_overhanging_float_in_child_ : 1;
  unsigned is_in_placed_tree_ : 1;
  DISALLOW_COPY_AND_ASSIGN(FloatingObject);
};

class FloatingObjectSet final : std::list<std::unique_ptr<FloatingObject>> {
public:
  using const_iterator = std::list<std::unique_ptr<FloatingObject>>::const_iterator;
  using iterator       = std::list<std::unique_ptr<FloatingObject>>::iterator;

  using std::list<std::unique_ptr<FloatingObject>>::back;
  using std::list<std::unique_ptr<FloatingObject>>::begin;
  using std::list<std::unique_ptr<FloatingObject>>::end;

  void clear(void) {
    indices_.clear();
    std::list<std::unique_ptr<FloatingObject>>::clear();
  }
  iterator insert(std::unique_ptr<FloatingObject> floating_object) {
    const LayoutBox *layout_box = floating_object->GetLayoutObject();
    auto it = std::list<std::unique_ptr<FloatingObject>>::insert(this->end(),
        std::move(floating_object));
    indices_[layout_box] = it;
    return it;
  }

  bool Contains(const FloatingObject *floating_object) const {
    return Find(floating_object->GetLayoutObject()) != this->end();
  }
  const_iterator Find(const LayoutBox *layout_box) const {
    auto it = indices_.find(layout_box);
    if (indices_.end() == it)
      return this->end();
    return it->second;
  }
  bool IsEmpty(void) const {
    return this->empty();
  }
  std::unique_ptr<FloatingObject> Take(FloatingObject *to_be_removed) {
    std::unique_ptr<FloatingObject> ret;
    const LayoutBox *layout_box = to_be_removed->GetLayoutObject();
    auto it = indices_.find(layout_box);
    if (indices_.end() != it) {
      ret = std::move(*it->second);
      std::list<std::unique_ptr<FloatingObject>>::erase(it->second);
    }
    return ret;
  }
  std::unique_ptr<FloatingObject> TakeFirst(void) {
    std::unique_ptr<FloatingObject> ret = std::move(this->front());
    this->pop_front();
    return ret;
  }
private:
  std::unordered_map<const LayoutBox *, iterator> indices_;
};

typedef FloatingObjectSet::const_iterator FloatingObjectSetIterator;
typedef WTF::PODInterval<LayoutUnit, FloatingObject*> FloatingObjectInterval;
typedef WTF::PODIntervalTree<LayoutUnit, FloatingObject*> FloatingObjectTree;
typedef WTF::PODFreeListArena<
    WTF::PODRedBlackTree<FloatingObjectInterval>::Node>
    IntervalArena;
typedef HashMap<LayoutBox*, std::unique_ptr<FloatingObject>>
    LayoutBoxToFloatInfoMap;

class FloatingObjects {
  USING_FAST_MALLOC(FloatingObjects);

 public:
  FloatingObjects(const LayoutBlockFlow*, bool horizontal_writing_mode);
  ~FloatingObjects();

  void Clear();
  void MoveAllToFloatInfoMap(LayoutBoxToFloatInfoMap&);
  FloatingObject* Add(std::unique_ptr<FloatingObject>);
  void Remove(FloatingObject*);
  void AddPlacedObject(FloatingObject&);
  void RemovePlacedObject(FloatingObject&);
  void SetHorizontalWritingMode(bool b = true) { horizontal_writing_mode_ = b; }

  bool HasLeftObjects() const { return left_objects_count_ > 0; }
  bool HasRightObjects() const { return right_objects_count_ > 0; }
  const FloatingObjectSet& Set() const { return set_; }
  FloatingObjectSet& MutableSet() { return set_; }
  void ClearLineBoxTreePointers();

  LayoutUnit LogicalLeftOffset(LayoutUnit fixed_offset,
                               LayoutUnit logical_top,
                               LayoutUnit logical_height);
  LayoutUnit LogicalRightOffset(LayoutUnit fixed_offset,
                                LayoutUnit logical_top,
                                LayoutUnit logical_height);

  LayoutUnit LogicalLeftOffsetForPositioningFloat(LayoutUnit fixed_offset,
                                                  LayoutUnit logical_top,
                                                  LayoutUnit* height_remaining);
  LayoutUnit LogicalRightOffsetForPositioningFloat(
      LayoutUnit fixed_offset,
      LayoutUnit logical_top,
      LayoutUnit* height_remaining);

  LayoutUnit LogicalLeftOffsetForAvoidingFloats(LayoutUnit fixed_offset,
                                                LayoutUnit logical_top,
                                                LayoutUnit logical_height);
  LayoutUnit LogicalRightOffsetForAvoidingFloats(LayoutUnit fixed_offset,
                                                 LayoutUnit logical_top,
                                                 LayoutUnit logical_height);

  LayoutUnit FindNextFloatLogicalBottomBelow(LayoutUnit logical_height);
  LayoutUnit FindNextFloatLogicalBottomBelowForBlock(LayoutUnit logical_height);

  LayoutUnit LowestFloatLogicalBottom(FloatingObject::Type);
  FloatingObject* LowestFloatingObject() const;

 private:
  bool HasLowestFloatLogicalBottomCached(bool is_horizontal,
                                         FloatingObject::Type float_type) const;
  LayoutUnit GetCachedlowestFloatLogicalBottom(
      FloatingObject::Type float_type) const;
  void SetCachedLowestFloatLogicalBottom(bool is_horizontal,
                                         FloatingObject::Type float_type,
                                         FloatingObject*);
  void MarkLowestFloatLogicalBottomCacheAsDirty();

  void ComputePlacedFloatsTree();
  const FloatingObjectTree& PlacedFloatsTree() {
    if (!placed_floats_tree_.IsInitialized())
      ComputePlacedFloatsTree();
    return placed_floats_tree_;
  }
  void IncreaseObjectsCount(FloatingObject::Type);
  void DecreaseObjectsCount(FloatingObject::Type);
  FloatingObjectInterval IntervalForFloatingObject(FloatingObject&);

  FloatingObjectSet set_;
  FloatingObjectTree placed_floats_tree_;
  unsigned left_objects_count_;
  unsigned right_objects_count_;
  bool horizontal_writing_mode_;
  const LayoutBlockFlow* layout_object_;

  struct FloatBottomCachedValue {
    FloatBottomCachedValue();
    FloatingObject* floating_object;
    bool dirty;
  };
  FloatBottomCachedValue lowest_float_bottom_cache_[2];
  bool cached_horizontal_writing_mode_;
  DISALLOW_COPY_AND_ASSIGN(FloatingObjects);
};

}  // namespace blink

namespace WTF {
#ifndef NDEBUG
// These structures are used by PODIntervalTree for debugging purposes.
template <>
struct ValueToString<blink::LayoutUnit> {
  static String ToString(const blink::LayoutUnit value);
};
template <>
struct ValueToString<blink::FloatingObject*> {
  static String ToString(const blink::FloatingObject*);
};
#endif
}  // namespace WTF

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_LAYOUT_FLOATING_OBJECTS_H_
