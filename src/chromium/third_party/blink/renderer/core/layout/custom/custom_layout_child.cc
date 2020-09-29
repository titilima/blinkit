// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: custom_layout_child.cc
// Description: CustomLayoutChild Class
//      Author: Ziming Li
//     Created: 2020-09-29
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/layout/custom/custom_layout_child.h"

// BKTODO: #include "third_party/blink/renderer/bindings/core/v8/serialization/serialized_script_value.h"
#include "third_party/blink/renderer/core/css/cssom/prepopulated_computed_style_property_map.h"
#if 0 // BKTODO:
#include "third_party/blink/renderer/core/layout/custom/css_layout_definition.h"
#include "third_party/blink/renderer/core/layout/custom/custom_layout_fragment_request.h"
#endif
#include "third_party/blink/renderer/core/layout/layout_box.h"

namespace blink {

CustomLayoutChild::CustomLayoutChild(const CSSLayoutDefinition& definition,
                                     LayoutBox* box)
#if 0 // BKTODO:
    : box_(box),
      style_map_(new PrepopulatedComputedStylePropertyMap(
          box->GetDocument(),
          box->StyleRef(),
          box->GetNode(),
          definition.ChildNativeInvalidationProperties(),
          definition.ChildCustomInvalidationProperties())) {}
#else
    : box_(box) {
  ASSERT(false); // BKTODO:
}
#endif

#if 0 // BKTODO:
CustomLayoutFragmentRequest* CustomLayoutChild::layoutNextFragment(
    ScriptState* script_state,
    const CustomLayoutConstraintsOptions& options,
    ExceptionState& exception_state) {
  // Serialize the provided data if needed.
  scoped_refptr<SerializedScriptValue> constraint_data;
  if (options.hasData()) {
    // We serialize "kForStorage" so that SharedArrayBuffers can't be shared
    // between LayoutWorkletGlobalScopes.
    constraint_data = SerializedScriptValue::Serialize(
        script_state->GetIsolate(), options.data().V8Value(),
        SerializedScriptValue::SerializeOptions(
            SerializedScriptValue::kForStorage),
        exception_state);

    if (exception_state.HadException())
      return nullptr;
  }

  return new CustomLayoutFragmentRequest(this, options,
                                         std::move(constraint_data));
}
#endif

}  // namespace blink
