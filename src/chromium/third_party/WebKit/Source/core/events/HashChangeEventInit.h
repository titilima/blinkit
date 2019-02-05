// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: HashChangeEventInit.h
// Description: HashChangeEventInit Class
//      Author: Ziming Li
//     Created: 2018-07-24
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_HASH_CHANGE_EVENT_INIT_H
#define BLINKIT_BLINK_HASH_CHANGE_EVENT_INIT_H

#pragma once

#include "core/events/EventInit.h"
#include "wtf/text/WTFString.h"

namespace blink {

class HashChangeEventInit : public EventInit
{
public:
    bool hasOldURL(void) const { return !m_oldURL.isNull(); }
    const String& oldURL(void) const { return m_oldURL; }

    bool hasNewURL(void) const { return !m_newURL.isNull(); }
    const String& newURL(void) const { return m_newURL; }
private:
    String m_oldURL, m_newURL;
};

} // namespace blink

#endif // BLINKIT_BLINK_HASH_CHANGE_EVENT_INIT_H
