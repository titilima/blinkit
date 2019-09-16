// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: dom_window.h
// Description: DOMWindow Class
//      Author: Ziming Li
//     Created: 2019-09-16
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BLINKIT_BLINK_DOM_WINDOW_H
#define BLINKIT_BLINK_DOM_WINDOW_H

#pragma once

#include "third_party/blink/renderer/core/dom/events/event_target.h"
#include "third_party/blink/renderer/core/frame/frame.h"

namespace blink {

class Frame;

class DOMWindow : public EventTargetWithInlineData
{
public:
    ~DOMWindow(void) override;

    Frame* GetFrame(void) const {
        assert(!m_frame || m_frame->DomWindow() == this);
        return m_frame;
    }
    bool IsLocalDOMWindow(void) const { return true; }
protected:
    explicit DOMWindow(Frame &frame);
private:
    Member<Frame> m_frame;
};

}  // namespace blink

#endif  // BLINKIT_BLINK_DOM_WINDOW_H
