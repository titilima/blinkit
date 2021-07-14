// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: ScrollToOptions.h
// Description: ScrollToOptions Class
//      Author: Ziming Li
//     Created: 2018-07-24
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_SCROLL_TO_OPTIONS_H
#define BLINKIT_BLINKIT_SCROLL_TO_OPTIONS_H

#pragma once

#include <optional>
#include "blinkit/blink/renderer/core/frame/ScrollOptions.h"

namespace blink {

class ScrollToOptions : public ScrollOptions
{
public:
    bool hasLeft(void) const { return m_left.has_value(); }
    double left(void) const
    {
        ASSERT(hasLeft());
        return m_left.value();
    }
    void setLeft(double l) { m_left = l; }

    bool hasTop(void) const { return m_top.has_value(); }
    double top(void) const
    {
        ASSERT(hasTop());
        return m_top.value();
    }
    void setTop(double t) { m_top = t; }
private:
    std::optional<double> m_left, m_top;
};

} // namespace blink

#endif // BLINKIT_BLINKIT_SCROLL_TO_OPTIONS_H
