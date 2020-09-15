// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: css_rotate.h
// Description: CSSRotate Class
//      Author: Ziming Li
//     Created: 2020-09-14
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_CSS_CSSOM_CSS_ROTATE_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_CSS_CSSOM_CSS_ROTATE_H_

#include "base/macros.h"
#include "third_party/blink/renderer/core/css/cssom/css_numeric_value.h"
#include "third_party/blink/renderer/core/css/cssom/css_transform_component.h"

namespace blink {

class DOMMatrix;
class ExceptionState;
class CSSNumericValue;

// Represents a rotation value in a CSSTransformValue used for properties like
// "transform".
// See CSSRotate.idl for more information about this class.
class CORE_EXPORT CSSRotate final : public CSSTransformComponent {
  DEFINE_WRAPPERTYPEINFO();

 public:
  // Constructors defined in the IDL.
  static CSSRotate* Create(CSSNumericValue* angle, ExceptionState&);
#if 0 // BKTODO:
  static CSSRotate* Create(const CSSNumberish& x,
                           const CSSNumberish& y,
                           const CSSNumberish& z,
                           CSSNumericValue* angle,
                           ExceptionState&);
#endif

  // Blink-internal ways of creating CSSRotates.
  static CSSRotate* Create(CSSNumericValue* angle);
  static CSSRotate* Create(CSSNumericValue* x,
                           CSSNumericValue* y,
                           CSSNumericValue* z,
                           CSSNumericValue* angle);
  static CSSRotate* FromCSSValue(const CSSFunctionValue&);

  // Getters and setters for attributes defined in the IDL.
  CSSNumericValue* angle() { return angle_.Get(); }
  void setAngle(CSSNumericValue* angle, ExceptionState&);
#if 0 // BKTODO:
  void x(CSSNumberish& x) { x.SetCSSNumericValue(x_); }
  void y(CSSNumberish& y) { y.SetCSSNumericValue(y_); }
  void z(CSSNumberish& z) { z.SetCSSNumericValue(z_); }
  void setX(const CSSNumberish&, ExceptionState&);
  void setY(const CSSNumberish&, ExceptionState&);
  void setZ(const CSSNumberish&, ExceptionState&);
#endif

  DOMMatrix* toMatrix(ExceptionState&) const final;

  // Internal methods - from CSSTransformComponent.
  TransformComponentType GetType() const final { return kRotationType; }
  const CSSFunctionValue* ToCSSValue() const final;

 private:
  CSSRotate(CSSNumericValue* x,
            CSSNumericValue* y,
            CSSNumericValue* z,
            CSSNumericValue* angle,
            bool is2D);

  Member<CSSNumericValue> angle_;
  Member<CSSNumericValue> x_;
  Member<CSSNumericValue> y_;
  Member<CSSNumericValue> z_;
  DISALLOW_COPY_AND_ASSIGN(CSSRotate);
};

}  // namespace blink

#endif
