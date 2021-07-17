// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: ElementAnimations.cpp
// Description: ElementAnimations Class
//      Author: Ziming Li
//     Created: 2021-07-17
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2013 Google Inc. All rights reserved.
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

#include "core/animation/ElementAnimations.h"

#include "core/layout/LayoutObject.h"

namespace blink {

ElementAnimations::ElementAnimations()
    : m_animationStyleChange(false)
{
}

ElementAnimations::~ElementAnimations()
{
#if !ENABLE(OILPAN)
    ASSERT(m_effects.isEmpty());
#endif
}

#if !ENABLE(OILPAN)
void ElementAnimations::dispose()
{
    // The notifyElementDestroyed() is called for elements that happen to live
    // longer than the KeyframeEffect. This undeterminism is fine because
    // all the notifyElementDestroyed() does is to clear the raw pointers
    // held by the KeyframeEffect.
    for (KeyframeEffect* effect : m_effects)
        effect->notifyElementDestroyed();
    m_effects.clear();
}
#endif

void ElementAnimations::updateAnimationFlags(ComputedStyle& style)
{
    for (const auto& entry : m_animations) {
        const Animation& animation = *entry.first;
        ASSERT(animation.effect());
        // FIXME: Needs to consider AnimationGroup once added.
        ASSERT(animation.effect()->isKeyframeEffect());
        const KeyframeEffect& effect = *toKeyframeEffect(animation.effect());
        if (effect.isCurrent()) {
            if (effect.affects(PropertyHandle(CSSPropertyOpacity)))
                style.setHasCurrentOpacityAnimation(true);
            if (effect.affects(PropertyHandle(CSSPropertyTransform))
                || effect.affects(PropertyHandle(CSSPropertyRotate))
                || effect.affects(PropertyHandle(CSSPropertyScale))
                || effect.affects(PropertyHandle(CSSPropertyTranslate)))
                style.setHasCurrentTransformAnimation(true);
            if (effect.affects(PropertyHandle(CSSPropertyWebkitFilter)))
                style.setHasCurrentFilterAnimation(true);
            if (effect.affects(PropertyHandle(CSSPropertyBackdropFilter)))
                style.setHasCurrentBackdropFilterAnimation(true);
        }
    }

    if (style.hasCurrentOpacityAnimation())
        style.setIsRunningOpacityAnimationOnCompositor(m_animationStack.hasActiveAnimationsOnCompositor(CSSPropertyOpacity));
    if (style.hasCurrentTransformAnimation())
        style.setIsRunningTransformAnimationOnCompositor(m_animationStack.hasActiveAnimationsOnCompositor(CSSPropertyTransform));
    if (style.hasCurrentFilterAnimation())
        style.setIsRunningFilterAnimationOnCompositor(m_animationStack.hasActiveAnimationsOnCompositor(CSSPropertyWebkitFilter));
    if (style.hasCurrentBackdropFilterAnimation())
        style.setIsRunningBackdropFilterAnimationOnCompositor(m_animationStack.hasActiveAnimationsOnCompositor(CSSPropertyBackdropFilter));
}

void ElementAnimations::restartAnimationOnCompositor()
{
    for (const auto& entry : m_animations)
        entry.first->restartAnimationOnCompositor();
}

DEFINE_TRACE(ElementAnimations)
{
    visitor->trace(m_cssAnimations);
    visitor->trace(m_animationStack);
    visitor->trace(m_animations);
#if !ENABLE(OILPAN)
    visitor->trace(m_effects);
#endif
}

const ComputedStyle* ElementAnimations::baseComputedStyle() const
{
#if !ENABLE(ASSERT)
    if (isAnimationStyleChange())
        return m_baseComputedStyle.get();
#endif
    return nullptr;
}

void ElementAnimations::updateBaseComputedStyle(const ComputedStyle* computedStyle)
{
    if (!isAnimationStyleChange()) {
        m_baseComputedStyle = nullptr;
        return;
    }
#if ENABLE(ASSERT)
    if (m_baseComputedStyle && computedStyle)
        ASSERT(*m_baseComputedStyle == *computedStyle);
#endif
    m_baseComputedStyle = ComputedStyle::clone(*computedStyle);
}

void ElementAnimations::clearBaseComputedStyle()
{
    m_baseComputedStyle = nullptr;
}

bool ElementAnimations::isAnimationStyleChange() const
{
    // TODO(rune@opera.com): The FontFaceCache version number may be increased without forcing
    // a style recalc (see crbug.com/471079). ComputedStyle objects created with different cache
    // versions will not be considered equal as Font::operator== will compare versions, hence
    // ComputedStyle::operator== will return false. We avoid using baseComputedStyle (the check for
    // isFallbackValid()) in that case to avoid triggering the ComputedStyle comparison ASSERT
    // in updateBaseComputedStyle.
    return m_animationStyleChange && (!m_baseComputedStyle || m_baseComputedStyle->font().isFallbackValid());
}

} // namespace blink
