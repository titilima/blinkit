// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: ComputedStyleCSSValueMapping.h
// Description: ComputedStyleCSSValueMapping Class
//      Author: Ziming Li
//     Created: 2021-10-18
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ComputedStyleCSSValueMapping_h
#define ComputedStyleCSSValueMapping_h

#include "core/CSSPropertyNames.h"
#include "core/css/CSSValue.h"
#include "wtf/Allocator.h"

namespace blink {

class LayoutObject;
class ComputedStyle;
class FilterOperations;
class ShadowData;
class ShadowList;
class StyleColor;
class Node;
class CSSVariableData;

class ComputedStyleCSSValueMapping {
    STATIC_ONLY(ComputedStyleCSSValueMapping);
public:
    // FIXME: Resolve computed auto alignment in applyProperty/ComputedStyle and remove this non-const styledNode parameter.
    static GCRefPtr<CSSValue> get(CSSPropertyID, const ComputedStyle&, const LayoutObject* = nullptr, Node* styledNode = nullptr, bool allowVisitedStyle = false);
    static GCRefPtr<CSSValue> get(const AtomicString customPropertyName, const ComputedStyle&);
    static const HashMap<AtomicString, RefPtr<CSSVariableData>>* getVariables(const ComputedStyle&);
private:
    static GCRefPtr<CSSValue> currentColorOrValidColor(const ComputedStyle&, const StyleColor&);
    static GCRefPtr<CSSValue> valueForShadowData(const ShadowData&, const ComputedStyle&, bool useSpread);
    static GCRefPtr<CSSValue> valueForShadowList(const ShadowList*, const ComputedStyle&, bool useSpread);
    static GCRefPtr<CSSValue> valueForFilter(const ComputedStyle&, const FilterOperations&);
    static GCRefPtr<CSSValue> valueForFont(const ComputedStyle&);
};

} // namespace blink

#endif // ComputedStyleCSSValueMapping_h
