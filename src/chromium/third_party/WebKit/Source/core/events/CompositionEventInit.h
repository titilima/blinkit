// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: CompositionEventInit.h
// Description: CompositionEventInit Class
//      Author: Ziming Li
//     Created: 2018-07-30
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_COMPOSITION_EVENT_INIT_H
#define BLINKIT_BLINK_COMPOSITION_EVENT_INIT_H

#pragma once

#include "core/events/UIEventInit.h"
#include "wtf/text/WTFString.h"

namespace blink {

class CompositionEventInit : public UIEventInit
{
public:
    bool hasData(void) const { return !m_data.isNull(); }
    const String& data(void) const { return m_data; }
    void setData(const String &value) { m_data = value; }
private:
    String m_data{ "" };
};

} // namespace blink

#endif // BLINKIT_BLINK_COMPOSITION_EVENT_INIT_H
