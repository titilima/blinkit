// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: style_sheet_list.cc
// Description: StyleSheetList Class
//      Author: Ziming Li
//     Created: 2020-09-24
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

/**
 * (C) 1999-2003 Lars Knoll (knoll@kde.org)
 * Copyright (C) 2004, 2006, 2007 Apple Inc. All rights reserved.
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

#include "third_party/blink/renderer/core/css/style_sheet_list.h"

#include "third_party/blink/renderer/core/css/style_engine.h"
#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/frame/use_counter.h"
// BKTODO: #include "third_party/blink/renderer/core/html/html_style_element.h"
#include "third_party/blink/renderer/core/html_names.h"
#include "third_party/blink/renderer/platform/wtf/text/wtf_string.h"

namespace blink {

using namespace html_names;

StyleSheetList* StyleSheetList::Create() {
  DCHECK(RuntimeEnabledFeatures::ConstructableStylesheetsEnabled());
  ASSERT(false); // BKTODO:
  return nullptr;
#if 0
  return new StyleSheetList();
#endif
}

StyleSheetList* StyleSheetList::Create(
    const HeapVector<Member<CSSStyleSheet>>& style_sheet_vector,
    ExceptionState& exception_state) {
  if (!RuntimeEnabledFeatures::ConstructableStylesheetsEnabled()) {
    exception_state.ThrowTypeError("Illegal constructor");
    return nullptr;
  }
  ASSERT(false); // BKTODO:
  return nullptr;
#if 0
  return new StyleSheetList(style_sheet_vector);
#endif
}

StyleSheetList::StyleSheetList(
    const HeapVector<Member<CSSStyleSheet>>& style_sheet_vector)
    : style_sheet_vector_(style_sheet_vector) {}

StyleSheetList::StyleSheetList(TreeScope* tree_scope)
    : tree_scope_(tree_scope) {
  CHECK(tree_scope);
}

#if 0 // BKTODO:
inline const HeapVector<TraceWrapperMember<StyleSheet>>&
StyleSheetList::StyleSheets() const {
  return GetDocument()->GetStyleEngine().StyleSheetsForStyleSheetList(
      *tree_scope_);
}
#endif

unsigned StyleSheetList::length() {
  if (!tree_scope_)
    return style_sheet_vector_.size();
  ASSERT(false); // BKTODO:
  return 0;
#if 0
  return StyleSheets().size();
#endif
}

StyleSheet* StyleSheetList::item(unsigned index) {
  if (!tree_scope_) {
    return index < style_sheet_vector_.size() ? style_sheet_vector_[index].Get()
                                              : nullptr;
  }
  ASSERT(false); // BKTODO:
  return nullptr;
#if 0
  const HeapVector<TraceWrapperMember<StyleSheet>>& sheets = StyleSheets();
  return index < sheets.size() ? sheets[index].Get() : nullptr;
#endif
}

HTMLStyleElement* StyleSheetList::GetNamedItem(const AtomicString& name) const {
  if (!tree_scope_)
    return nullptr;

  // IE also supports retrieving a stylesheet by name, using the name/id of the
  // <style> tag (this is consistent with all the other collections) ### Bad
  // implementation because returns a single element (are IDs always unique?)
  // and doesn't look for name attribute. But unicity of stylesheet ids is good
  // practice anyway ;)
  // FIXME: We should figure out if we should change this or fix the spec.
  Element* element = tree_scope_->getElementById(name);
  ASSERT(false); // BKTODO:
  return nullptr;
#if 0
  return IsHTMLStyleElement(element) ? ToHTMLStyleElement(element) : nullptr;
#endif
}

CSSStyleSheet* StyleSheetList::AnonymousNamedGetter(const AtomicString& name) {
  if (GetDocument()) {
    UseCounter::Count(*GetDocument(),
                      WebFeature::kStyleSheetListAnonymousNamedGetter);
  }
  HTMLStyleElement* item = GetNamedItem(name);
  if (!item)
    return nullptr;
  ASSERT(false); // BKTODO:
  return nullptr;
#if 0
  CSSStyleSheet* sheet = item->sheet();
  if (sheet) {
    UseCounter::Count(*GetDocument(),
                      WebFeature::kStyleSheetListNonNullAnonymousNamedGetter);
  }
  return sheet;
#endif
}

}  // namespace blink
