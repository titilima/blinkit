/*
 * Copyright (c) 2010, Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "platform/scroll/ScrollAnimatorBase.h"

#include "platform/RuntimeEnabledFeatures.h"
#include "platform/geometry/FloatPoint.h"
#include "platform/scroll/ScrollableArea.h"
#include "wtf/MathExtras.h"
#include "wtf/PassOwnPtr.h"

namespace blink {

ScrollAnimatorBase::ScrollAnimatorBase(ScrollableArea* scrollableArea)
    : m_scrollableArea(scrollableArea)
    , m_currentPosX(0)
    , m_currentPosY(0)
{
}

ScrollAnimatorBase::~ScrollAnimatorBase()
{
}

float ScrollAnimatorBase::computeDeltaToConsume(ScrollbarOrientation orientation, float pixelDelta) const
{
    float currentPos = (orientation == HorizontalScrollbar) ? m_currentPosX : m_currentPosY;
    float newPos = clampScrollPosition(orientation, currentPos + pixelDelta);
    return (currentPos == newPos) ? 0.0f : (newPos - currentPos);
}

ScrollResultOneDimensional ScrollAnimatorBase::userScroll(ScrollbarOrientation orientation, ScrollGranularity, float step, float delta)
{
    float& currentPos = (orientation == HorizontalScrollbar) ? m_currentPosX : m_currentPosY;
    float usedPixelDelta = computeDeltaToConsume(orientation, step * delta);
    float newPos = currentPos + usedPixelDelta;
    if (currentPos == newPos)
        return ScrollResultOneDimensional(false, delta);

    currentPos = newPos;

    notifyPositionChanged();

    return ScrollResultOneDimensional(true, delta - (usedPixelDelta / step));
}

void ScrollAnimatorBase::scrollToOffsetWithoutAnimation(const FloatPoint& offset)
{
    m_currentPosX = offset.x();
    m_currentPosY = offset.y();
    notifyPositionChanged();
}

void ScrollAnimatorBase::setCurrentPosition(const FloatPoint& position)
{
    m_currentPosX = position.x();
    m_currentPosY = position.y();
}

FloatPoint ScrollAnimatorBase::currentPosition() const
{
    return FloatPoint(m_currentPosX, m_currentPosY);
}

void ScrollAnimatorBase::notifyPositionChanged()
{
    m_scrollableArea->scrollPositionChanged(DoublePoint(m_currentPosX, m_currentPosY), UserScroll);
}

float ScrollAnimatorBase::clampScrollPosition(ScrollbarOrientation orientation, float pos) const
{
    float maxScrollPos = m_scrollableArea->maximumScrollPosition(orientation);
    float minScrollPos = m_scrollableArea->minimumScrollPosition(orientation);
    return clampTo(pos, minScrollPos, maxScrollPos);
}

DEFINE_TRACE(ScrollAnimatorBase)
{
    visitor->trace(m_scrollableArea);
    ScrollAnimatorCompositorCoordinator::trace(visitor);
}

} // namespace blink
