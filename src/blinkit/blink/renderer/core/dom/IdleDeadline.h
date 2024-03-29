#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: IdleDeadline.h
// Description: IdleDeadline Class
//      Author: Ziming Li
//     Created: 2022-03-02
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef IdleDeadline_h
#define IdleDeadline_h

#include "blinkit/blink/renderer/bindings/core/script_wrappable.h"

namespace blink {

class IdleDeadline : public GarbageCollected<IdleDeadline>, public ScriptWrappable
{
    DEFINE_WRAPPERTYPEINFO();
public:
    enum class CallbackType {
        CalledWhenIdle,
        CalledByTimeout
    };

    static IdleDeadline* create(double deadlineSeconds, CallbackType callbackType)
    {
        return new IdleDeadline(deadlineSeconds, callbackType);
    }

    DEFINE_INLINE_TRACE() { }

    double timeRemaining() const;

    bool didTimeout() const
    {
        return m_callbackType == CallbackType::CalledByTimeout;
    }

private:
    IdleDeadline(double deadlineSeconds, CallbackType);

    double m_deadlineSeconds;
    CallbackType m_callbackType;
};

} // namespace blink

#endif // IdleDeadline_h
