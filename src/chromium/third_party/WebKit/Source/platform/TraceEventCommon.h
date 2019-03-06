// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: TraceEventCommon.h
// Description: Placeholders for Trace Macros
//      Author: Ziming Li
//     Created: 2018-08-18
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_TRACE_EVENT_COMMON_H
#define BLINKIT_BLINK_TRACE_EVENT_COMMON_H

#pragma once

// Flags for changing the behavior of TRACE_EVENT_API_ADD_TRACE_EVENT.
#define TRACE_EVENT_FLAG_NONE (static_cast<unsigned int>(0))
#define TRACE_EVENT_FLAG_COPY (static_cast<unsigned int>(1 << 0))
#define TRACE_EVENT_FLAG_HAS_ID (static_cast<unsigned int>(1 << 1))
#define TRACE_EVENT_FLAG_MANGLE_ID (static_cast<unsigned int>(1 << 2))
#define TRACE_EVENT_FLAG_SCOPE_OFFSET (static_cast<unsigned int>(1 << 3))
#define TRACE_EVENT_FLAG_SCOPE_EXTRA (static_cast<unsigned int>(1 << 4))
#define TRACE_EVENT_FLAG_EXPLICIT_TIMESTAMP (static_cast<unsigned int>(1 << 5))
#define TRACE_EVENT_FLAG_ASYNC_TTS (static_cast<unsigned int>(1 << 6))
#define TRACE_EVENT_FLAG_BIND_TO_ENCLOSING (static_cast<unsigned int>(1 << 7))
#define TRACE_EVENT_FLAG_FLOW_IN (static_cast<unsigned int>(1 << 8))
#define TRACE_EVENT_FLAG_FLOW_OUT (static_cast<unsigned int>(1 << 9))
#define TRACE_EVENT_FLAG_HAS_CONTEXT_ID (static_cast<unsigned int>(1 << 10))

// Type values for identifying types in the TraceValue union.
#define TRACE_VALUE_TYPE_BOOL (static_cast<unsigned char>(1))
#define TRACE_VALUE_TYPE_UINT (static_cast<unsigned char>(2))
#define TRACE_VALUE_TYPE_INT (static_cast<unsigned char>(3))
#define TRACE_VALUE_TYPE_DOUBLE (static_cast<unsigned char>(4))
#define TRACE_VALUE_TYPE_POINTER (static_cast<unsigned char>(5))
#define TRACE_VALUE_TYPE_STRING (static_cast<unsigned char>(6))
#define TRACE_VALUE_TYPE_COPY_STRING (static_cast<unsigned char>(7))
#define TRACE_VALUE_TYPE_CONVERTABLE (static_cast<unsigned char>(8))

// Enum reflecting the scope of an INSTANT event. Must fit within
// TRACE_EVENT_FLAG_SCOPE_MASK.
#define TRACE_EVENT_SCOPE_GLOBAL (static_cast<unsigned char>(0 << 3))
#define TRACE_EVENT_SCOPE_PROCESS (static_cast<unsigned char>(1 << 3))
#define TRACE_EVENT_SCOPE_THREAD (static_cast<unsigned char>(2 << 3))

#define TRACE_COUNTER1(...)
#define TRACE_EVENT0(...)
#define TRACE_EVENT1(...)
#define TRACE_EVENT2(...)
#define TRACE_EVENT_ASYNC_BEGIN0(...)
#define TRACE_EVENT_ASYNC_BEGIN1(...)
#define TRACE_EVENT_ASYNC_BEGIN2(...)
#define TRACE_EVENT_ASYNC_END0(...)
#define TRACE_EVENT_ASYNC_END1(...)
#define TRACE_EVENT_ASYNC_END2(...)
#define TRACE_EVENT_ASYNC_STEP_INTO0(...)
#define TRACE_EVENT_ASYNC_STEP_INTO1(...)
#define TRACE_EVENT_BEGIN0(...)
#define TRACE_EVENT_BEGIN1(...)
#define TRACE_EVENT_BEGIN2(...)
#define TRACE_EVENT_CATEGORY_GROUP_ENABLED(a, b)    (*(b) = false)
#define TRACE_EVENT_END0(...)
#define TRACE_EVENT_END1(...)
#define TRACE_EVENT_END2(...)
#define TRACE_EVENT_GET_SAMPLING_STATE()    (nullptr)
#define TRACE_EVENT_INSTANT1(...)
#define TRACE_EVENT_MARK_WITH_TIMESTAMP0(...)
#define TRACE_EVENT_MARK_WITH_TIMESTAMP1(...)
#define TRACE_EVENT_NESTABLE_ASYNC_BEGIN1(...)
#define TRACE_EVENT_NESTABLE_ASYNC_END1(...)
#define TRACE_EVENT_NESTABLE_ASYNC_INSTANT1(...)
#define TRACE_EVENT_OBJECT_CREATED_WITH_ID(...)
#define TRACE_EVENT_OBJECT_DELETED_WITH_ID(...)
#define TRACE_EVENT_OBJECT_SNAPSHOT_WITH_ID(...)
#define TRACE_EVENT_SCOPED_SAMPLING_STATE(...)
#define TRACE_EVENT_SET_NONCONST_SAMPLING_STATE(a)  ((void)0)
#define TRACE_EVENT_SET_SAMPLING_STATE(a, b)    ((void)0)

#endif // BLINKIT_BLINK_TRACE_EVENT_COMMON_H
