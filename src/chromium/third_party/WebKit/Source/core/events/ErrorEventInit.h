// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: ErrorEventInit.h
// Description: ErrorEventInit Class
//      Author: Ziming Li
//     Created: 2018-07-23
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_ERROR_EVENT_INIT_H
#define BLINKIT_BLINK_ERROR_EVENT_INIT_H

#pragma once

#include <optional>
#include "core/events/EventInit.h"
#include "wtf/text/WTFString.h"

namespace blink {

class ErrorEventInit : public EventInit
{
public:
    bool hasColno(void) const { return m_colno.has_value(); }
    unsigned colno(void) const { return m_colno.value(); }
    void setColno(unsigned value) { m_colno = value; }

    bool hasFilename(void) const { return !m_filename.isNull(); }
    const String& filename(void) const { return m_filename; }
    void setFilename(const String &value) { m_filename = value; }

    bool hasLineno(void) const { return m_lineno.has_value(); }
    unsigned lineno(void) const { return m_lineno.value(); }
    void setLineno(unsigned value) { m_lineno = value; }

    bool hasMessage(void) const { return !m_message.isNull(); }
    const String& message(void) const { return m_message; }
    void setMessage(const String &value) { m_message = value; }
private:
    std::optional<unsigned> m_colno, m_lineno;
    String m_filename, m_message;
};

} // namespace blink

#endif // BLINKIT_BLINK_ERROR_EVENT_INIT_H
