// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: css_transform_value.cc
// Description: CSSTransformValue Class
//      Author: Ziming Li
//     Created: 2020-09-14
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/css/cssom/css_transform_value.h"

#include "third_party/blink/renderer/core/css/css_value_list.h"
#include "third_party/blink/renderer/core/css/cssom/css_transform_component.h"
#if 0 // BKTODO:
#include "third_party/blink/renderer/core/geometry/dom_matrix.h"
#else
#include "third_party/blink/renderer/platform/bindings/exception_state.h"
#endif
#include "third_party/blink/renderer/platform/bindings/exception_messages.h"
#include "third_party/blink/renderer/platform/wtf/text/string_builder.h"

namespace blink {

CSSTransformValue* CSSTransformValue::Create(
    const HeapVector<Member<CSSTransformComponent>>& transform_components,
    ExceptionState& exception_state) {
  CSSTransformValue* value = Create(transform_components);
  if (!value) {
    exception_state.ThrowTypeError(
        "CSSTransformValue must have at least one component");
    return nullptr;
  }
  return value;
}

CSSTransformValue* CSSTransformValue::Create(
    const HeapVector<Member<CSSTransformComponent>>& transform_components) {
  if (transform_components.IsEmpty())
    return nullptr;
  return new CSSTransformValue(transform_components);
}

CSSTransformValue* CSSTransformValue::FromCSSValue(const CSSValue& css_value) {
  if (!css_value.IsValueList()) {
    // TODO(meade): Also need to check the separator here if we care.
    return nullptr;
  }
  HeapVector<Member<CSSTransformComponent>> components;
  for (const CSSValue* value : ToCSSValueList(css_value)) {
    CSSTransformComponent* component =
        CSSTransformComponent::FromCSSValue(*value);
    if (!component)
      return nullptr;
    components.push_back(component);
  }
  return CSSTransformValue::Create(components);
}

bool CSSTransformValue::is2D() const {
  return std::all_of(transform_components_.begin(), transform_components_.end(),
                     [](const auto& component) { return component->is2D(); });
}

DOMMatrix* CSSTransformValue::toMatrix(ExceptionState& exception_state) const {
  ASSERT(false); // BKTODO:
  return nullptr;
#if 0
  DOMMatrix* matrix = DOMMatrix::Create();
  for (wtf_size_t i = 0; i < transform_components_.size(); i++) {
    const DOMMatrix* matrixComponent =
        transform_components_[i]->toMatrix(exception_state);
    if (matrixComponent) {
      matrix->multiplySelf(*matrixComponent);
    }
  }
  return matrix;
#endif
}

const CSSValue* CSSTransformValue::ToCSSValue() const {
  CSSValueList* transform_css_value = CSSValueList::CreateSpaceSeparated();
  for (wtf_size_t i = 0; i < transform_components_.size(); i++) {
    const CSSValue* component = transform_components_[i]->ToCSSValue();
    // TODO(meade): Remove this check once numbers and lengths are rewritten.
    if (!component)
      return nullptr;
    transform_css_value->Append(*component);
  }
  return transform_css_value;
}

bool CSSTransformValue::AnonymousIndexedSetter(
    unsigned index,
    const Member<CSSTransformComponent> component,
    ExceptionState& exception_state) {
  if (index < transform_components_.size()) {
    transform_components_[index] = component;
    return true;
  }

  if (index == transform_components_.size()) {
    transform_components_.push_back(component);
    return true;
  }

  exception_state.ThrowRangeError(
      ExceptionMessages::IndexOutsideRange<unsigned>(
          "index", index, 0, ExceptionMessages::kInclusiveBound,
          transform_components_.size(), ExceptionMessages::kInclusiveBound));
  return false;
}

}  // namespace blink
