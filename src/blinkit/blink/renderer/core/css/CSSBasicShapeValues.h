// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: CSSBasicShapeValues.h
// Description: CSS Basic Shape Value Classes
//      Author: Ziming Li
//     Created: 2021-07-18
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2011 Adobe Systems Incorporated. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer.
 * 2. Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef CSSBasicShapeValues_h
#define CSSBasicShapeValues_h

#include "core/css/CSSPrimitiveValue.h"
#include "core/css/CSSValue.h"
#include "core/css/CSSValuePair.h"
#include "platform/graphics/GraphicsTypes.h"
#include "wtf/RefPtr.h"
#include "wtf/Vector.h"
#include "wtf/text/WTFString.h"

namespace blink {

class CSSBasicShapeCircleValue final : public CSSValue {
public:
    static GCRefPtr<CSSBasicShapeCircleValue> create() { return BlinKit::GCWrapShared(new CSSBasicShapeCircleValue); }

    String customCSSText() const;
    bool equals(const CSSBasicShapeCircleValue&) const;

    CSSValue* centerX() const { return m_centerX.get(); }
    CSSValue* centerY() const { return m_centerY.get(); }
    CSSPrimitiveValue* radius() const { return m_radius.get(); }

    // TODO(sashab): Remove these and pass them as arguments in the constructor.
    void setCenterX(CSSValue *centerX) { m_centerX = centerX; }
    void setCenterX(const GCRefPtr<CSSValue> &centerX) { m_centerX = centerX; }
    void setCenterY(CSSValue *centerY) { m_centerY = centerY; }
    void setCenterY(const GCRefPtr<CSSValue> &centerY) { m_centerY = centerY; }
    void setRadius(CSSPrimitiveValue *radius) { m_radius = radius; }
    void setRadius(const GCRefPtr<CSSPrimitiveValue> &radius) { m_radius = radius; }

    DECLARE_TRACE_AFTER_DISPATCH();

private:
    CSSBasicShapeCircleValue()
        : CSSValue(BasicShapeCircleClass)
        { }

    GCRefPtr<CSSValue> m_centerX;
    GCRefPtr<CSSValue> m_centerY;
    GCRefPtr<CSSPrimitiveValue> m_radius;
};

class CSSBasicShapeEllipseValue final : public CSSValue {
public:
    static GCRefPtr<CSSBasicShapeEllipseValue> create() { return BlinKit::GCWrapShared(new CSSBasicShapeEllipseValue); }

    String customCSSText() const;
    bool equals(const CSSBasicShapeEllipseValue&) const;

    CSSValue* centerX() const { return m_centerX.get(); }
    CSSValue* centerY() const { return m_centerY.get(); }
    CSSPrimitiveValue* radiusX() const { return m_radiusX.get(); }
    CSSPrimitiveValue* radiusY() const { return m_radiusY.get(); }

    // TODO(sashab): Remove these and pass them as arguments in the constructor.
    void setCenterX(CSSValue *centerX) { m_centerX = centerX; }
    void setCenterX(const GCRefPtr<CSSValue> &centerX) { m_centerX = centerX; }
    void setCenterY(CSSValue *centerY) { m_centerY = centerY; }
    void setCenterY(const GCRefPtr<CSSValue> &centerY) { m_centerY = centerY; }
    void setRadiusX(CSSPrimitiveValue *radiusX) { m_radiusX = radiusX; }
    void setRadiusX(const GCRefPtr<CSSPrimitiveValue> &radiusX) { m_radiusX = radiusX; }
    void setRadiusY(CSSPrimitiveValue *radiusY) { m_radiusY = radiusY; }
    void setRadiusY(const GCRefPtr<CSSPrimitiveValue> &radiusY) { m_radiusY = radiusY; }

    DECLARE_TRACE_AFTER_DISPATCH();

private:
    CSSBasicShapeEllipseValue()
        : CSSValue(BasicShapeEllipseClass)
        { }

    GCRefPtr<CSSValue> m_centerX;
    GCRefPtr<CSSValue> m_centerY;
    GCRefPtr<CSSPrimitiveValue> m_radiusX;
    GCRefPtr<CSSPrimitiveValue> m_radiusY;
};

class CSSBasicShapePolygonValue final : public CSSValue {
public:
    static GCRefPtr<CSSBasicShapePolygonValue> create() { return BlinKit::GCWrapShared(new CSSBasicShapePolygonValue); }

    void appendPoint(CSSPrimitiveValue *x, CSSPrimitiveValue *y)
    {
        m_values.emplace_back(x);
        m_values.emplace_back(y);
    }
    void appendPoint(const GCRefPtr<CSSPrimitiveValue> &x, const GCRefPtr<CSSPrimitiveValue> &y)
    {
        m_values.emplace_back(x);
        m_values.emplace_back(y);
    }

    CSSPrimitiveValue* getXAt(unsigned i) const { return m_values.at(i * 2).get(); }
    CSSPrimitiveValue* getYAt(unsigned i) const { return m_values.at(i * 2 + 1).get(); }
    const std::vector<GCRefPtr<CSSPrimitiveValue>>& values() const { return m_values; }

    // TODO(sashab): Remove this and pass it as an argument in the constructor.
    void setWindRule(WindRule w) { m_windRule = w; }
    WindRule windRule() const { return m_windRule; }

    String customCSSText() const;
    bool equals(const CSSBasicShapePolygonValue&) const;

    DECLARE_TRACE_AFTER_DISPATCH();

private:
    CSSBasicShapePolygonValue()
        : CSSValue(BasicShapePolygonClass),
        m_windRule(RULE_NONZERO)
    { }

    std::vector<GCRefPtr<CSSPrimitiveValue>> m_values;
    WindRule m_windRule;
};

class CSSBasicShapeInsetValue final : public CSSValue {
public:
    static GCRefPtr<CSSBasicShapeInsetValue> create() { return BlinKit::GCWrapShared(new CSSBasicShapeInsetValue); }

    CSSPrimitiveValue* top() const { return m_top.get(); }
    CSSPrimitiveValue* right() const { return m_right.get(); }
    CSSPrimitiveValue* bottom() const { return m_bottom.get(); }
    CSSPrimitiveValue* left() const { return m_left.get(); }

    CSSValuePair* topLeftRadius() const { return m_topLeftRadius.get(); }
    CSSValuePair* topRightRadius() const { return m_topRightRadius.get(); }
    CSSValuePair* bottomRightRadius() const { return m_bottomRightRadius.get(); }
    CSSValuePair* bottomLeftRadius() const { return m_bottomLeftRadius.get(); }

    // TODO(sashab): Remove these and pass them as arguments in the constructor.
    void setTop(CSSPrimitiveValue *top) { m_top = top; }
    void setTop(const GCRefPtr<CSSPrimitiveValue> &top) { m_top = top; }
    void setRight(CSSPrimitiveValue *right) { m_right = right; }
    void setRight(const GCRefPtr<CSSPrimitiveValue> &right) { m_right = right; }
    void setBottom(CSSPrimitiveValue *bottom) { m_bottom = bottom; }
    void setBottom(const GCRefPtr<CSSPrimitiveValue> &bottom) { m_bottom = bottom; }
    void setLeft(CSSPrimitiveValue *left) { m_left = left; }
    void setLeft(const GCRefPtr<CSSPrimitiveValue> &left) { m_left = left; }

    void updateShapeSize4Values(CSSPrimitiveValue* top, CSSPrimitiveValue* right, CSSPrimitiveValue* bottom, CSSPrimitiveValue* left)
    {
        setTop(top);
        setRight(right);
        setBottom(bottom);
        setLeft(left);
    }

    void updateShapeSize1Value(CSSPrimitiveValue* value1)
    {
        updateShapeSize4Values(value1, value1, value1, value1);
    }

    void updateShapeSize2Values(CSSPrimitiveValue* value1,  CSSPrimitiveValue* value2)
    {
        updateShapeSize4Values(value1, value2, value1, value2);
    }

    void updateShapeSize3Values(CSSPrimitiveValue* value1, CSSPrimitiveValue* value2,  CSSPrimitiveValue* value3)
    {
        updateShapeSize4Values(value1, value2, value3, value2);
    }

    void setTopLeftRadius(const GCRefPtr<CSSValuePair> &radius) { m_topLeftRadius = radius; }
    void setTopRightRadius(const GCRefPtr<CSSValuePair> &radius) { m_topRightRadius = radius; }
    void setBottomRightRadius(const GCRefPtr<CSSValuePair> &radius) { m_bottomRightRadius = radius; }
    void setBottomLeftRadius(const GCRefPtr<CSSValuePair> &radius) { m_bottomLeftRadius = radius; }

    String customCSSText() const;
    bool equals(const CSSBasicShapeInsetValue&) const;

    DECLARE_TRACE_AFTER_DISPATCH();

private:
    CSSBasicShapeInsetValue()
        : CSSValue(BasicShapeInsetClass)
        { }

    GCRefPtr<CSSPrimitiveValue> m_top;
    GCRefPtr<CSSPrimitiveValue> m_right;
    GCRefPtr<CSSPrimitiveValue> m_bottom;
    GCRefPtr<CSSPrimitiveValue> m_left;

    GCRefPtr<CSSValuePair> m_topLeftRadius;
    GCRefPtr<CSSValuePair> m_topRightRadius;
    GCRefPtr<CSSValuePair> m_bottomRightRadius;
    GCRefPtr<CSSValuePair> m_bottomLeftRadius;
};

DEFINE_CSS_VALUE_TYPE_CASTS(CSSBasicShapeCircleValue, isBasicShapeCircleValue());
DEFINE_CSS_VALUE_TYPE_CASTS(CSSBasicShapeEllipseValue, isBasicShapeEllipseValue());
DEFINE_CSS_VALUE_TYPE_CASTS(CSSBasicShapePolygonValue, isBasicShapePolygonValue());
DEFINE_CSS_VALUE_TYPE_CASTS(CSSBasicShapeInsetValue, isBasicShapeInsetValue());

} // namespace blink

#endif // CSSBasicShapeValues_h
