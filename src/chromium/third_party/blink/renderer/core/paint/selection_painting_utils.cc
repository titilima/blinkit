// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: selection_painting_utils.cc
// Description: SelectionPaintingUtils Class
//      Author: Ziming Li
//     Created: 2020-10-07
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/paint/selection_painting_utils.h"

#include "third_party/blink/renderer/core/css/pseudo_style_request.h"
#include "third_party/blink/renderer/core/dom/element.h"
#include "third_party/blink/renderer/core/dom/element_traversal.h"
#include "third_party/blink/renderer/core/dom/node.h"
#include "third_party/blink/renderer/core/dom/shadow_root.h"
#include "third_party/blink/renderer/core/editing/frame_selection.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/layout/layout_theme.h"
#include "third_party/blink/renderer/core/paint/paint_info.h"
#include "third_party/blink/renderer/core/paint/text_paint_style.h"
#include "third_party/blink/renderer/core/style/computed_style.h"
#include "third_party/blink/renderer/platform/graphics/color.h"

namespace blink {

namespace {

bool NodeIsSelectable(const ComputedStyle& style, Node* node) {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return !node->IsInert() && !(style.UserSelect() == EUserSelect::kNone &&
                               style.UserModify() == EUserModify::kReadOnly);
#endif
}

scoped_refptr<ComputedStyle> GetUncachedSelectionStyle(Node* node) {
  if (!node)
    return nullptr;

  // In Blink, ::selection only applies to direct children of the element on
  // which ::selection is matched. In order to be able to style ::selection
  // inside elements implemented with a UA shadow tree, like input::selection,
  // we calculate ::selection style on the shadow host for elements inside the
  // UA shadow.
  if (ShadowRoot* root = node->ContainingShadowRoot()) {
    ASSERT(false); // BKTODO:
#if 0
    if (root->IsUserAgent()) {
      if (Element* shadow_host = node->OwnerShadowHost()) {
        return shadow_host->StyleForPseudoElement(
            PseudoStyleRequest(kPseudoIdSelection));
      }
    }
#endif
  }

  // If we request ::selection style for LayoutText, query ::selection style on
  // the parent element instead, as that is the node for which ::selection
  // matches.
  Element* element = Traversal<Element>::FirstAncestorOrSelf(*node);
  if (!element || element->IsPseudoElement())
    return nullptr;

  ASSERT(false); // BKTODO:
  return nullptr;
#if 0
  return element->StyleForPseudoElement(PseudoStyleRequest(kPseudoIdSelection));
#endif
}

Color SelectionColor(const Document& document,
                     const ComputedStyle& style,
                     Node* node,
                     const CSSProperty& color_property,
                     const GlobalPaintFlags global_paint_flags) {
  // If the element is unselectable, or we are only painting the selection,
  // don't override the foreground color with the selection foreground color.
  if ((node && !NodeIsSelectable(style, node)) ||
      (global_paint_flags & kGlobalPaintSelectionOnly))
    return style.VisitedDependentColor(color_property);

  if (scoped_refptr<ComputedStyle> pseudo_style =
          GetUncachedSelectionStyle(node))
    return pseudo_style->VisitedDependentColor(color_property);
  if (!LayoutTheme::GetTheme().SupportsSelectionForegroundColors())
    return style.VisitedDependentColor(color_property);
  ASSERT(false); // BKTODO:
  return style.VisitedDependentColor(color_property);
#if 0
  return document.GetFrame()->Selection().FrameIsFocusedAndActive()
             ? LayoutTheme::GetTheme().ActiveSelectionForegroundColor()
             : LayoutTheme::GetTheme().InactiveSelectionForegroundColor();
#endif
}

const ComputedStyle* SelectionPseudoStyle(Node* node) {
  if (!node)
    return nullptr;
  Element* element = Traversal<Element>::FirstAncestorOrSelf(*node);
  if (!element)
    return nullptr;
  ASSERT(false); // BKTODO:
  return nullptr;
#if 0
  return element->CachedStyleForPseudoElement(
      PseudoStyleRequest(kPseudoIdSelection));
#endif
}

}  // anonymous namespace

Color SelectionPaintingUtils::SelectionBackgroundColor(
    const Document& document,
    const ComputedStyle& style,
    Node* node) {
  if (node && !NodeIsSelectable(style, node))
    return Color::kTransparent;

  if (scoped_refptr<ComputedStyle> pseudo_style =
          GetUncachedSelectionStyle(node)) {
    return pseudo_style->VisitedDependentColor(GetCSSPropertyBackgroundColor())
        .BlendWithWhite();
  }

  ASSERT(false); // BKTODO:
  return Color::kTransparent;
#if 0
  return document.GetFrame()->Selection().FrameIsFocusedAndActive()
             ? LayoutTheme::GetTheme().ActiveSelectionBackgroundColor()
             : LayoutTheme::GetTheme().InactiveSelectionBackgroundColor();
#endif
}

Color SelectionPaintingUtils::SelectionForegroundColor(
    const Document& document,
    const ComputedStyle& style,
    Node* node,
    const GlobalPaintFlags global_paint_flags) {
  return SelectionColor(document, style, node,
                        GetCSSPropertyWebkitTextFillColor(),
                        global_paint_flags);
}

Color SelectionPaintingUtils::SelectionEmphasisMarkColor(
    const Document& document,
    const ComputedStyle& style,
    Node* node,
    const GlobalPaintFlags global_paint_flags) {
  return SelectionColor(document, style, node,
                        GetCSSPropertyWebkitTextEmphasisColor(),
                        global_paint_flags);
}

TextPaintStyle SelectionPaintingUtils::SelectionPaintingStyle(
    const Document& document,
    const ComputedStyle& style,
    Node* node,
    bool have_selection,
    const TextPaintStyle& text_style,
    const PaintInfo& paint_info) {
  TextPaintStyle selection_style = text_style;
  bool uses_text_as_clip = paint_info.phase == PaintPhase::kTextClip;
  bool is_printing = paint_info.IsPrinting();
  const GlobalPaintFlags global_paint_flags = paint_info.GetGlobalPaintFlags();

  if (have_selection) {
    if (!uses_text_as_clip) {
      selection_style.fill_color =
          SelectionForegroundColor(document, style, node, global_paint_flags);
      selection_style.emphasis_mark_color =
          SelectionEmphasisMarkColor(document, style, node, global_paint_flags);
    }

    if (const ComputedStyle* pseudo_style = SelectionPseudoStyle(node)) {
      selection_style.stroke_color =
          uses_text_as_clip ? Color::kBlack
                            : pseudo_style->VisitedDependentColor(
                                  GetCSSPropertyWebkitTextStrokeColor());
      selection_style.stroke_width = pseudo_style->TextStrokeWidth();
      selection_style.shadow =
          uses_text_as_clip ? nullptr : pseudo_style->TextShadow();
    }

    // Text shadows are disabled when printing. http://crbug.com/258321
    if (is_printing)
      selection_style.shadow = nullptr;
  }

  return selection_style;
}

}  // namespace blink
