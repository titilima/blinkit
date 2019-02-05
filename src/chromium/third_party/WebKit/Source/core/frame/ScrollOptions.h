// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: ScrollOptions.h
// Description: ScrollOptions Class
//      Author: Ziming Li
//     Created: 2018-07-24
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_SCROLL_OPTIONS_H
#define BLINKIT_BLINK_SCROLL_OPTIONS_H

#pragma once

#include "wtf/text/WTFString.h"

namespace blink {

class ScrollOptions
{
public:
    bool hasBehavior(void) const { return !m_behavior.isNull(); }
    const String& behavior(void) const { return m_behavior; }
private:
    String m_behavior;
};

} // namespace blink

#endif // BLINKIT_BLINK_SCROLL_OPTIONS_H
