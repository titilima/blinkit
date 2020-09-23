// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: shadow_tree_style_sheet_collection.cc
// Description: ShadowTreeStyleSheetCollection Class
//      Author: Ziming Li
//     Created: 2020-09-23
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2001 Dirk Mueller (mueller@kde.org)
 *           (C) 2006 Alexey Proskuryakov (ap@webkit.org)
 * Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2012 Apple Inc. All
 * rights reserved.
 * Copyright (C) 2008, 2009 Torch Mobile Inc. All rights reserved.
 * (http://www.torchmobile.com/)
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies)
 * Copyright (C) 2013 Google Inc. All rights reserved.
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

#include "third_party/blink/renderer/core/css/shadow_tree_style_sheet_collection.h"

#include "third_party/blink/renderer/core/css/css_style_sheet.h"
#include "third_party/blink/renderer/core/css/resolver/style_resolver.h"
#include "third_party/blink/renderer/core/css/style_change_reason.h"
#include "third_party/blink/renderer/core/css/style_engine.h"
#include "third_party/blink/renderer/core/css/style_sheet_candidate.h"
#include "third_party/blink/renderer/core/dom/element.h"
#include "third_party/blink/renderer/core/dom/shadow_root.h"
// BKTODO: #include "third_party/blink/renderer/core/html/html_style_element.h"
#include "third_party/blink/renderer/core/html_names.h"

namespace blink {

using namespace html_names;

ShadowTreeStyleSheetCollection::ShadowTreeStyleSheetCollection(
    ShadowRoot& shadow_root)
    : TreeScopeStyleSheetCollection(shadow_root) {}

void ShadowTreeStyleSheetCollection::CollectStyleSheets(
    StyleEngine& master_engine,
    StyleSheetCollection& collection) {
  for (Node* n : style_sheet_candidate_nodes_) {
    StyleSheetCandidate candidate(*n);
    DCHECK(!candidate.IsXSL());

    StyleSheet* sheet = candidate.Sheet();
    if (!sheet)
      continue;

    collection.AppendSheetForList(sheet);
    if (candidate.CanBeActivated(g_null_atom)) {
      CSSStyleSheet* css_sheet = ToCSSStyleSheet(sheet);
      collection.AppendActiveStyleSheet(
          std::make_pair(css_sheet, master_engine.RuleSetForSheet(*css_sheet)));
    }
  }
  ASSERT(false); // BKTODO:
#if 0
  if (!GetTreeScope().HasAdoptedStyleSheets())
    return;

  StyleSheetList& adopted_style_sheets = GetTreeScope().AdoptedStyleSheets();
  unsigned length = adopted_style_sheets.length();
  for (unsigned index = 0; index < length; ++index) {
    StyleSheet* sheet = adopted_style_sheets.item(index);
    if (!sheet)
      continue;
    CSSStyleSheet* css_sheet = ToCSSStyleSheet(sheet);
    if (!css_sheet || !css_sheet->CanBeActivated(g_null_atom))
      continue;
    collection.AppendActiveStyleSheet(
        std::make_pair(css_sheet, master_engine.RuleSetForSheet(*css_sheet)));
  }
#endif
}

void ShadowTreeStyleSheetCollection::UpdateActiveStyleSheets(
    StyleEngine& master_engine) {
  // StyleSheetCollection is GarbageCollected<>, allocate it on the heap.
  StyleSheetCollection* collection = StyleSheetCollection::Create();
  CollectStyleSheets(master_engine, *collection);
  ApplyActiveStyleSheetChanges(*collection);
}

}  // namespace blink
