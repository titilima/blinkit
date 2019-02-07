// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: RelatedEventInit.h
// Description: RelatedEventInit Class
//      Author: Ziming Li
//     Created: 2018-08-02
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_RELATED_EVENT_INIT_H
#define BLINKIT_BLINK_RELATED_EVENT_INIT_H

#pragma once

#include "core/events/EventInit.h"
#include "core/events/EventTarget.h"

namespace blink {

class RelatedEventInit : public EventInit
{
public:
    bool hasRelatedTarget(void) const { return m_relatedTarget; }
    PassRefPtrWillBeRawPtr<EventTarget> relatedTarget(void) const { return m_relatedTarget; }
    void setRelatedTarget(PassRefPtrWillBeRawPtr<EventTarget> value) { m_relatedTarget = value; }
private:
    RefPtrWillBeMember<EventTarget> m_relatedTarget;
};

} // namespace blink

#endif // BLINKIT_BLINK_RELATED_EVENT_INIT_H
