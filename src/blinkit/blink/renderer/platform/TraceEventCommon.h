#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: TraceEventCommon.h
// Description: Placeholders for Trace Macros
//      Author: Ziming Li
//     Created: 2018-08-18
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_TRACE_EVENT_COMMON_H
#define BLINKIT_BLINKIT_TRACE_EVENT_COMMON_H

#define TRACE_COUNTER1(...) ((void)0)

#define TRACE_EVENT_INSTANT1(...)   ((void)0)

#define TRACE_EVENT0(...)   ((void)0)
#define TRACE_EVENT1(...)   ((void)0)
#define TRACE_EVENT2(...)   ((void)0)

#define TRACE_EVENT_ASYNC_BEGIN1(...)   ((void)0)
#define TRACE_EVENT_ASYNC_BEGIN2(...)   ((void)0)
#define TRACE_EVENT_ASYNC_END0(...)     ((void)0)
#define TRACE_EVENT_ASYNC_END1(...)     ((void)0)
#define TRACE_EVENT_ASYNC_END2(...)     ((void)0)

#define TRACE_EVENT_ASYNC_STEP_INTO1(...)   ((void)0)

#define TRACE_EVENT_BEGIN0(...) ((void)0)
#define TRACE_EVENT_BEGIN1(...) ((void)0)
#define TRACE_EVENT_BEGIN2(...) ((void)0)
#define TRACE_EVENT_END0(...)   ((void)0)
#define TRACE_EVENT_END1(...)   ((void)0)
#define TRACE_EVENT_END2(...)   ((void)0)

#define TRACE_EVENT_CATEGORY_GROUP_ENABLED(...) ((void)0)

#define TRACE_EVENT_NESTABLE_ASYNC_BEGIN1(...)      ((void)0)
#define TRACE_EVENT_NESTABLE_ASYNC_END1(...)        ((void)0)
#define TRACE_EVENT_NESTABLE_ASYNC_INSTANT1(...)    ((void)0)

#define TRACE_EVENT_OBJECT_SNAPSHOT_WITH_ID(...)    ((void)0)

#define TRACE_EVENT_SCOPED_SAMPLING_STATE(...)  ((void)0)

#define TRACE_EVENT_SET_SAMPLING_STATE(...) ((void)0)

#endif // BLINKIT_BLINKIT_TRACE_EVENT_COMMON_H
