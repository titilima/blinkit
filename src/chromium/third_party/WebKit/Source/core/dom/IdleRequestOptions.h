// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: IdleRequestOptions.h
// Description: IdleRequestOptions Class
//      Author: Ziming Li
//     Created: 2019-02-07
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_IDLE_REQUEST_OPTIONS_H
#define BLINKIT_BLINK_IDLE_REQUEST_OPTIONS_H

#pragma once

namespace blink {

class IdleRequestOptions
{
public:
    unsigned long timeout(void) const { return m_timeout; }
private:
    unsigned long m_timeout = 0;
};

} // namespace blink

#endif // BLINKIT_BLINK_IDLE_REQUEST_OPTIONS_H
