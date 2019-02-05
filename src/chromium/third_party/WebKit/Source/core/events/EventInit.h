// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: EventInit.h
// Description: EventInit Class
//      Author: Ziming Li
//     Created: 2018-07-12
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_EVENT_INIT_H
#define BLINKIT_BLINK_EVENT_INIT_H

#pragma once

namespace blink {

class EventInit
{
public:
    bool hasBubbles(void) const { return true; }
    bool bubbles(void) const { return m_bubbles; }
    void setBubbles(bool value) { m_bubbles = value; }

    bool hasCancelable(void) const { return true; }
    bool cancelable(void) const { return m_cancelable; }
    void setCancelable(bool value) { m_cancelable = value; }
private:
    bool m_bubbles = false, m_cancelable = false;
};

} // namespace blink

#endif // BLINKIT_BLINK_EVENT_INIT_H
