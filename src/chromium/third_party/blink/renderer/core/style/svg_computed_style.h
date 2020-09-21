// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: svg_computed_style.h
// Description: Placeholders for SVG
//      Author: Ziming Li
//     Created: 2020-09-21
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_SVG_COMPUTED_STYLE_H
#define BLINKIT_BLINK_SVG_COMPUTED_STYLE_H

#pragma once

#include "third_party/blink/renderer/core/style/svg_computed_style_defs.h"
#include "third_party/blink/renderer/platform/length.h"

namespace blink {

class SVGComputedStyle
{
public:
    void NotReached(void) { NOTREACHED(); }

    static StylePath* InitialD(void) { return nullptr; }
    StylePath* D(void) const
    {
        NOTREACHED();
        return nullptr;
    }
    void SetD(scoped_refptr<StylePath>) { NOTREACHED(); }

    static Length InitialX(void) { return Length(kFixed); }
    Length X(void) const
    {
        NOTREACHED();
        return InitialX();
    }
    void SetX(const Length &) { NOTREACHED(); }

    static Length InitialY(void) { return Length(kFixed); }
    Length Y(void) const
    {
        NOTREACHED();
        return InitialY();
    }
    void SetY(const Length &) { NOTREACHED(); }
};

} // namespace blink

#define SetAlignmentBaseline(...)           NotReached()
#define SetBaselineShift(...)               NotReached()
#define SetBufferedRendering(...)           NotReached()
#define SetCapStyle(...)                    NotReached()
#define SetClipRule(...)                    NotReached()
#define SetColorInterpolation(...)          NotReached()
#define SetColorInterpolationFilters(...)   NotReached()
#define SetColorRendering(...)              NotReached()
#define SetCx(...)                          NotReached()
#define SetCy(...)                          NotReached()
#define SetDominantBaseline(...)            NotReached()
#define SetFillOpacity(...)                 NotReached()
#define SetFillPaint(...)                   NotReached()
#define SetFillRule(...)                    NotReached()
#define SetFloodColor(...)                  NotReached()
#define SetFloodOpacity(...)                NotReached()
#define SetJoinStyle(...)                   NotReached()
#define SetLightingColor(...)               NotReached()
#define SetMarkerEndResource(...)           NotReached()
#define SetMarkerMidResource(...)           NotReached()
#define SetMarkerStartResource(...)         NotReached()
#define SetMaskType(...)                    NotReached()
#define SetMaskerResource(...)              NotReached()
#define SetPaintOrder(...)                  NotReached()
#define SetR(...)                           NotReached()
#define SetRx(...)                          NotReached()
#define SetRy(...)                          NotReached()
#define SetShapeRendering(...)              NotReached()
#define SetStopColor(...)                   NotReached()
#define SetStopOpacity(...)                 NotReached()
#define SetStrokeDashArray(...)             NotReached()
#define SetStrokeDashOffset(...)            NotReached()
#define SetStrokeMiterLimit(...)            NotReached()
#define SetStrokeOpacity(...)               NotReached()
#define SetStrokePaint(...)                 NotReached()
#define SetStrokeWidth(...)                 NotReached()
#define SetTextAnchor(...)                  NotReached()
#define SetVectorEffect(...)                NotReached()
#define SetVisitedLinkFillPaint(...)        NotReached()
#define SetVisitedLinkStrokePaint(...)      NotReached()

#endif // BLINKIT_BLINK_SVG_COMPUTED_STYLE_H
