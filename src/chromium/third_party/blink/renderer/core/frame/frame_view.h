// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: frame_view.h
// Description: FrameView Class
//      Author: Ziming Li
//     Created: 2020-10-19
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_FRAME_VIEW_H
#define BLINKIT_BLINK_FRAME_VIEW_H

#pragma once

namespace blink {

class FrameView : public GarbageCollectedFinalized<FrameView>
{
public:
    BK_DECLARE_GC_NAME(FrameView)

    virtual ~FrameView(void) = default;
    virtual void Trace(Visitor *visitor) {}

    virtual void Show(void) = 0;
};

} // namespace blink

#endif // BLINKIT_BLINK_FRAME_VIEW_H
