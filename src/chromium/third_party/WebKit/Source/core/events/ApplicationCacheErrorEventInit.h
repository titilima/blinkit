// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: ApplicationCacheErrorEventInit.h
// Description: ApplicationCacheErrorEventInit Class
//      Author: Ziming Li
//     Created: 2018-08-02
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_APPLICATION_CACHE_ERROR_EVENT_INIT_H
#define BLINKIT_BLINK_APPLICATION_CACHE_ERROR_EVENT_INIT_H

#pragma once

#include <optional>
#include "core/events/EventInit.h"
#include "wtf/text/WTFString.h"

namespace blink {

class ApplicationCacheErrorEventInit : public EventInit
{
public:
    bool hasMessage(void) const { return !m_message.isNull(); }
    const String& message(void) const { return m_message; }
    void setMessage(const String &value) { m_message = value; }

    bool hasReason(void) const { return !m_reason.isNull(); }
    const String& reason(void) const { return m_reason; }
    void setReason(const String &value) { m_reason = value; }

    bool hasStatus(void) const { return m_status.has_value(); }
    unsigned status(void) const { return m_status.value(); }
    void setStatus(unsigned value) { m_status = value; }

    bool hasURL(void) const { return !m_url.isNull(); }
    const String& url(void) const { return m_url; }
    void setURL(const String &value) { m_url = value; }
private:
    String m_message;
    String m_reason;
    std::optional<unsigned> m_status;
    String m_url;
};

} // namespace blink

#endif // BLINKIT_BLINK_APPLICATION_CACHE_ERROR_EVENT_INIT_H
