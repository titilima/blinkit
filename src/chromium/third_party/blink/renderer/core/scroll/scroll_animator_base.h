// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: scroll_animator_base.h
// Description: WinThemeEngine Class
//      Author: Ziming Li
//     Created: 2019-03-15
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_SCROLL_ANIMATOR_BASE_H
#define BLINKIT_BLINK_SCROLL_ANIMATOR_BASE_H

#pragma once

namespace blink {

class Visitor;

class ScrollAnimatorBase : public GarbageCollectedFinalized<ScrollAnimatorBase>
{
public:
    static ScrollAnimatorBase* Create(ScrollableArea *)
    {
        return new ScrollAnimatorBase;
    }
    void Trace(Visitor *) {}

    void ContentsResized(void) const
    {
#ifdef OS_MACOSX
        static_cast(false); // BKTODO: ScrollAnimatorMac
#endif
}
    void DidAddVerticalScrollbar(Scrollbar &)
    {
#ifdef OS_MACOSX
        static_cast(false); // BKTODO: ScrollAnimatorMac
#endif
    }
    void WillRemoveVerticalScrollbar(Scrollbar &)
    {
#ifdef OS_MACOSX
        static_cast(false); // BKTODO: ScrollAnimatorMac
#endif
    }
    void DidAddHorizontalScrollbar(Scrollbar &)
    {
#ifdef OS_MACOSX
        static_cast(false); // BKTODO: ScrollAnimatorMac
#endif
    }
    void WillRemoveHorizontalScrollbar(Scrollbar &)
    {
#ifdef OS_MACOSX
        static_cast(false); // BKTODO: ScrollAnimatorMac
#endif
    }
};

} // namespace blink

#endif // BLINKIT_BLINK_SCROLL_ANIMATOR_BASE_H
