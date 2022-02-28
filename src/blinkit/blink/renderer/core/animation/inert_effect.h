#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: inert_effect.h
// Description: InertEffect Class
//      Author: Ziming Li
//     Created: 2021-11-01
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

#ifndef InertEffect_h
#define InertEffect_h

#include "blinkit/blink/renderer/core/animation/AnimationEffect.h"
#include "blinkit/blink/renderer/core/animation/EffectModel.h"
#include "blinkit/blink/renderer/wtf/RefPtr.h"

namespace blink {

class InertEffect final : public AnimationEffect
{
public:
    static GCRefPtr<InertEffect> create(EffectModel*, const Timing&, bool paused, double inheritedTime);
    void sample(std::vector<RefPtr<Interpolation>>&) const;
    EffectModel* model() const { return m_model.get(); }
    bool paused() const { return m_paused; }

    DECLARE_VIRTUAL_TRACE();

protected:
    void updateChildrenAndEffects() const override { }
    double calculateTimeToEffectChange(bool forwards, double inheritedTime, double timeToNextIteration) const override;

private:
    InertEffect(const GCRefPtr<EffectModel>&, const Timing&, bool paused, double inheritedTime);
    GCRefPtr<EffectModel> m_model;
    bool m_paused;
    double m_inheritedTime;
};

} // namespace blink

#endif
