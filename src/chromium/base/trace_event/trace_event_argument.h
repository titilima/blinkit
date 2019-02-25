// -------------------------------------------------
// BlinKit - base Library
// -------------------------------------------------
//   File Name: trace_event_argument.h
// Description: Placeholder for Trace Event Classes
//      Author: Ziming Li
//     Created: 2018-08-30
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BASE_TRACE_EVENT_ARGUMENT_H
#define BLINKIT_BASE_TRACE_EVENT_ARGUMENT_H

#pragma once

namespace base {
namespace trace_event {

class ConvertableToTraceFormat
{
public:
    void AddRef(void) {
        assert(false); // Not reached!
    }
    void Release(void) {
        assert(false); // Not reached!
    }
};

} // namespace trace_event
} // namespace base

#endif // BLINKIT_BASE_TRACE_EVENT_ARGUMENT_H
