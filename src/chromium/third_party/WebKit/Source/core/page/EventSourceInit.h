// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: EventSourceInit.h
// Description: EventSourceInit Class
//      Author: Ziming Li
//     Created: 2018-08-04
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_EVENT_SOURCE_INIT_H
#define BLINKIT_BLINK_EVENT_SOURCE_INIT_H

#pragma once

namespace blink {

class EventSourceInit
{
public:
    bool hasWithCredentials(void) const { return true; }
    bool withCredentials(void) const { return m_withCredentials; }
    void setWithCredentials(bool value) { m_withCredentials = value; }
private:
    bool m_withCredentials = false;
};

} // namespace blink

#endif // BLINKIT_BLINK_EVENT_SOURCE_INIT_H
