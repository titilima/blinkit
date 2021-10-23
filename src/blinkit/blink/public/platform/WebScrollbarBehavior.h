// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: WebScrollbarBehavior.h
// Description: WebScrollbarBehavior Class
//      Author: Ziming Li
//     Created: 2021-10-16
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WebScrollbarBehavior_h
#define WebScrollbarBehavior_h

namespace blink {

class IntPoint;
class IntRect;

class WebScrollbarBehavior {
public:
    enum Button {
        ButtonNone = -1,
        ButtonLeft,
        ButtonMiddle,
        ButtonRight
    };
    virtual ~WebScrollbarBehavior() { }
    virtual bool shouldCenterOnThumb(Button, bool shiftKeyPressed, bool altKeyPressed) { return false; }
    virtual bool shouldSnapBackToDragOrigin(const IntPoint& eventPoint, const IntRect& scrollbarRect, bool isHorizontal) { return false; }
};

} // namespace blink

#endif
