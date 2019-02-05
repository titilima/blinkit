// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: FocusEventInit.h
// Description: FocusEventInit Class
//      Author: Ziming Li
//     Created: 2018-07-24
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_FOCUS_EVENT_INIT_H
#define BLINKIT_BLINK_FOCUS_EVENT_INIT_H

#pragma once

#include "core/events/EventTarget.h"
#include "core/events/UIEventInit.h"

namespace blink {

class FocusEventInit : public UIEventInit
{
public:
    bool hasRelatedTarget(void) const { return m_relatedTarget; }
    PassRefPtrWillBeRawPtr<EventTarget> relatedTarget(void) const { return m_relatedTarget; }
    void setRelatedTarget(PassRefPtrWillBeRawPtr<EventTarget> value) { m_relatedTarget = value; }
private:
    RefPtrWillBeMember<EventTarget> m_relatedTarget;
};

} // namespace blink

#endif // BLINKIT_BLINK_FOCUS_EVENT_INIT_H
