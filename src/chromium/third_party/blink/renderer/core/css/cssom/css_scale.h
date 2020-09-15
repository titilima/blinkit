// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: css_scale.h
// Description: CSSScale Class
//      Author: Ziming Li
//     Created: 2020-09-14
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_CSS_CSSOM_CSS_SCALE_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_CSS_CSSOM_CSS_SCALE_H_

#include "base/macros.h"
#include "third_party/blink/renderer/core/css/cssom/css_transform_component.h"
#include "third_party/blink/renderer/core/css/cssom/css_unit_value.h"
#if 0 // BKTODO:
#include "third_party/blink/renderer/core/geometry/dom_matrix.h"
#else
#include "third_party/blink/renderer/platform/bindings/exception_state.h"
#endif

namespace blink {

class CSSNumericValue;
class DOMMatrix;

// Represents a scale value in a CSSTransformValue used for properties like
// "transform".
// See CSSScale.idl for more information about this class.
class CORE_EXPORT CSSScale final : public CSSTransformComponent {
  DEFINE_WRAPPERTYPEINFO();

 public:
#if 0 // BKTODO:
  // Constructors defined in the IDL.
  static CSSScale* Create(const CSSNumberish&,
                          const CSSNumberish&,
                          ExceptionState&);
  static CSSScale* Create(const CSSNumberish&,
                          const CSSNumberish&,
                          const CSSNumberish&,
                          ExceptionState&);
#endif

  // Blink-internal ways of creating CSSScales.
  static CSSScale* Create(CSSNumericValue* x, CSSNumericValue* y) {
    return new CSSScale(x, y, CSSUnitValue::Create(1), true /* is2D */);
  }
  static CSSScale* Create(CSSNumericValue* x,
                          CSSNumericValue* y,
                          CSSNumericValue* z) {
    return new CSSScale(x, y, z, false /* is2D */);
  }
  static CSSScale* FromCSSValue(const CSSFunctionValue&);

#if 0 // BKTODO:
  // Getters and setters for attributes defined in the IDL.
  void x(CSSNumberish& x) { x.SetCSSNumericValue(x_); }
  void y(CSSNumberish& y) { y.SetCSSNumericValue(y_); }
  void z(CSSNumberish& z) { z.SetCSSNumericValue(z_); }
  void setX(const CSSNumberish&, ExceptionState&);
  void setY(const CSSNumberish&, ExceptionState&);
  void setZ(const CSSNumberish&, ExceptionState&);
#endif

  DOMMatrix* toMatrix(ExceptionState&) const final;

  // Internal methods - from CSSTransformComponent.
  TransformComponentType GetType() const final { return kScaleType; }
  const CSSFunctionValue* ToCSSValue() const final;

 private:
  CSSScale(CSSNumericValue* x,
           CSSNumericValue* y,
           CSSNumericValue* z,
           bool is2D);

  Member<CSSNumericValue> x_;
  Member<CSSNumericValue> y_;
  Member<CSSNumericValue> z_;

  DISALLOW_COPY_AND_ASSIGN(CSSScale);
};

}  // namespace blink

#endif
