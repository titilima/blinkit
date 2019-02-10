// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: AutocompleteErrorEventInit.h
// Description: AutocompleteErrorEventInit Class
//      Author: Ziming Li
//     Created: 2019-02-09
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_AUTOCOMPLETE_ERROR_EVENT_INIT_H
#define BLINKIT_BLINK_AUTOCOMPLETE_ERROR_EVENT_INIT_H

#pragma once

#include "core/events/EventInit.h"
#include "wtf/text/WTFString.h"

namespace blink {

class AutocompleteErrorEventInit : public EventInit
{
public:
    bool hasReason(void) const { return !m_reason.isNull(); }
    const String& reason(void) const { return m_reason; }
private:
    String m_reason;
};

} // namespace blink

#endif // BLINKIT_BLINK_AUTOCOMPLETE_ERROR_EVENT_INIT_H
