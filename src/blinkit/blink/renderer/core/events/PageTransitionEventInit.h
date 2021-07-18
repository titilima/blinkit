// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: PageTransitionEventInit.h
// Description: PageTransitionEventInit Class
//      Author: Ziming Li
//     Created: 2018-07-24
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_PAGE_TRANSITION_EVENT_INIT_H
#define BLINKIT_BLINKIT_PAGE_TRANSITION_EVENT_INIT_H

#pragma once

#include <optional>
#include "core/events/EventInit.h"

namespace blink {

class PageTransitionEventInit : public EventInit
{
public:
    bool hasPersisted(void) const { return m_persisted.has_value(); }
    bool persisted(void) const {
        assert(m_persisted.has_value());
        return m_persisted.value();
    }
    void setPersisted(bool value) { m_persisted = value; }
private:
    std::optional<bool> m_persisted;
};

} // namespace blink

#endif // BLINKIT_BLINKIT_PAGE_TRANSITION_EVENT_INIT_H
