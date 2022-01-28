#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: web_layer_position_constraint.h
// Description: WebLayerPositionConstraint Struct
//      Author: Ziming Li
//     Created: 2021-12-07
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2013 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef WebLayerPositionConstraint_h
#define WebLayerPositionConstraint_h

namespace blink {

struct WebLayerPositionConstraint {
    bool isFixedPosition     : 1;
    bool isFixedToRightEdge  : 1;
    bool isFixedToBottomEdge : 1;

    WebLayerPositionConstraint()
        : isFixedPosition(false)
        , isFixedToRightEdge(false)
        , isFixedToBottomEdge(false)
    {
    }

    static WebLayerPositionConstraint fixedPosition(bool fixedToRightEdge, bool fixedToBottomEdge)
    {
        WebLayerPositionConstraint constraint;
        constraint.isFixedPosition = true;
        constraint.isFixedToRightEdge = fixedToRightEdge;
        constraint.isFixedToBottomEdge = fixedToBottomEdge;
        return constraint;
    }

    bool operator==(const WebLayerPositionConstraint &o) const
    {
        return isFixedPosition == o.isFixedPosition
            && isFixedToRightEdge == o.isFixedToRightEdge
            && isFixedToBottomEdge == o.isFixedToBottomEdge;
    }

#ifndef NDEBUG
    bool IsDefault(void) const { return !(isFixedPosition || isFixedToRightEdge || isFixedToBottomEdge); }
#endif
};

} // namespace blink

#ifndef NDEBUG
namespace zed {
template <>
inline void log_serializer::push<blink::WebLayerPositionConstraint>(
    std::vector<std::string> &dst,
    const blink::WebLayerPositionConstraint &positionConstraint)
{
    std::string s(1, '[');
    if (positionConstraint.isFixedPosition)
        s.append(" FixedPosition,");
    if (positionConstraint.isFixedToRightEdge)
        s.append(" FixedToRightEdge,");
    if (positionConstraint.isFixedToBottomEdge)
        s.append(" FixedToBottomEdge,");
    if (s.length() > 1)
        s.back() = ' ';
    s.push_back(']');

    dst.emplace_back(s);
}
}
#endif

#endif // WebLayerPositionConstraint_h
