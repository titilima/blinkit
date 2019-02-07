// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: ProgressEventInit.h
// Description: ProgressEventInit Class
//      Author: Ziming Li
//     Created: 2018-08-02
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_PROGRESS_EVENT_INIT_H
#define BLINKIT_BLINK_PROGRESS_EVENT_INIT_H

#pragma once

#include "core/events/EventInit.h"

namespace blink {

class ProgressEventInit : public EventInit
{
public:
    bool hasLengthComputable(void) const { return true; }
    bool lengthComputable(void) const { return m_lengthComputable; }
    void setLengthComputable(bool value) { m_lengthComputable = value; }

    bool hasLoaded(void) const { return true; }
    unsigned long long loaded(void) const { return m_loaded; }
    void setLoaded(unsigned long long value) { m_loaded = value; }

    bool hasTotal(void) const { return true; }
    unsigned long long total(void) const { return m_total; }
    void setTotal(unsigned long long value) { m_total = value; }
private:
    bool m_lengthComputable = false;
    unsigned long long m_loaded = 0;
    unsigned long long m_total = 0;
};

} // namespace blink

#endif // BLINKIT_BLINK_PROGRESS_EVENT_INIT_H
