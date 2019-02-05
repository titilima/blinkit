// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: TouchEventInit.h
// Description: TouchEventInit Class
//      Author: Ziming Li
//     Created: 2018-07-12
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_TOUCH_EVENT_INIT_H
#define BLINKIT_BLINK_TOUCH_EVENT_INIT_H

#pragma once

#include "core/events/EventModifierInit.h"
#include "platform/heap/Handle.h"

namespace blink {

class TouchEventInit : public EventModifierInit
{
public:
    const auto& touches(void) const { return m_touches; }

    const auto& targetTouches(void) const { return m_targetTouches; }

    const auto& changedTouches(void) const { return m_changedTouches; }
private:
    WillBeHeapVector<RefPtrWillBeMember<Touch>> m_touches;
    WillBeHeapVector<RefPtrWillBeMember<Touch>> m_targetTouches;
    WillBeHeapVector<RefPtrWillBeMember<Touch>> m_changedTouches;
};

} // namespace blink

#endif // BLINKIT_BLINK_TOUCH_EVENT_INIT_H
