// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: text.h
// Description: Text Class
//      Author: Ziming Li
//     Created: 2019-10-30
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 * Copyright (C) 2003, 2004, 2005, 2006, 2007, 2008, 2009 Apple Inc. All rights
 * reserved.
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

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_DOM_TEXT_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_DOM_TEXT_H_

#include "third_party/blink/renderer/core/core_export.h"
#include "third_party/blink/renderer/core/dom/character_data.h"

namespace blink {

class LayoutText;
class WhitespaceAttacher;

class CORE_EXPORT Text : public CharacterData {
  DEFINE_WRAPPERTYPEINFO();

 public:
  static const unsigned kDefaultLengthLimit = 1 << 16;

  static Text* Create(Document&, const String&);
#ifndef BLINKIT_CRAWLER_ONLY
  static Text* CreateEditingText(Document&, const String&);
#endif

  LayoutText* GetLayoutObject() const;

  // mergeNextSiblingNodesIfPossible() merges next sibling nodes if possible
  // then returns a node not merged.
  Node* MergeNextSiblingNodesIfPossible();
  Text* splitText(unsigned offset, ExceptionState&);

  // DOM Level 3: http://www.w3.org/TR/DOM-Level-3-Core/core.html#ID-1312295772

  String wholeText() const;
  Text* ReplaceWholeText(const String&);

#ifndef BLINKIT_CRAWLER_ONLY
  void RecalcTextStyle(StyleRecalcChange);
  void RebuildTextLayoutTree(WhitespaceAttacher&);
  bool TextLayoutObjectIsNeeded(const AttachContext&,
                                const ComputedStyle&,
                                const LayoutObject& parent) const;
  LayoutText* CreateTextLayoutObject(const ComputedStyle&);
  void UpdateTextLayoutObject(unsigned offset_of_replaced_data,
                              unsigned length_of_replaced_data);

  void AttachLayoutTree(AttachContext&) final;
  void ReattachLayoutTreeIfNeeded(const AttachContext&);

  bool CanContainRangeEndPoint() const final { return true; }
#endif
  NodeType getNodeType() const override;

 protected:
  Text(TreeScope& tree_scope, const String& data, ConstructionType type)
      : CharacterData(tree_scope, data, type) {}

 private:
  String nodeName() const override;
  Node* Clone(Document&, CloneChildrenFlag) const override;

  bool IsTextNode() const =
      delete;  // This will catch anyone doing an unnecessary check.

  bool NeedsWhitespaceLayoutObject();

  virtual Text* CloneWithData(Document&, const String&) const;
};

DEFINE_NODE_TYPE_CASTS(Text, IsTextNode());

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_DOM_TEXT_H_
