// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: focus_options.h
// Description: FocusOptions Class
//      Author: Ziming Li
//     Created: 2021-02-05
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_FOCUS_OPTIONS_H
#define BLINKIT_BLINK_FOCUS_OPTIONS_H

#pragma once

namespace blink {

class FocusOptions final
{
public:
    FocusOptions(void) = default;

    bool preventScroll(void) const { return m_preventScroll; }
    void setPreventScroll(bool b) { m_preventScroll = b; }
private:
    bool m_preventScroll = false;
};

} // namespace blink

#endif // BLINKIT_BLINK_FOCUS_OPTIONS_H
