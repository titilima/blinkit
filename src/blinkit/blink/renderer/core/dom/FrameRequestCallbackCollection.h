// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: FrameRequestCallbackCollection.h
// Description: FrameRequestCallbackCollection Class
//      Author: Ziming Li
//     Created: 2021-07-21
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FrameRequestCallbackCollection_h
#define FrameRequestCallbackCollection_h

#include "core/CoreExport.h"
#include "platform/heap/Handle.h"

namespace blink {

class ExecutionContext;
class FrameRequestCallback;

class CORE_EXPORT FrameRequestCallbackCollection final {
    DISALLOW_NEW();
public:
    explicit FrameRequestCallbackCollection(ExecutionContext*);

    using CallbackId = int;
    CallbackId registerCallback(FrameRequestCallback*);
    void cancelCallback(CallbackId);
    void executeCallbacks(double highResNowMs, double highResNowMsLegacy);

    bool isEmpty() const { return !m_callbacks.size(); }

    DECLARE_TRACE();

private:
    using CallbackList = std::vector<Member<FrameRequestCallback>>;
    CallbackList m_callbacks;
    CallbackList m_callbacksToInvoke; // only non-empty while inside executeCallbacks

    CallbackId m_nextCallbackId = 0;

    ExecutionContext *m_context;
};

} // namespace blink

#endif // FrameRequestCallbackCollection_h
