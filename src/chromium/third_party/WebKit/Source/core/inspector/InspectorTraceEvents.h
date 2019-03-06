// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: InspectorTraceEvents.h
// Description: Placeholders for Inspector
//      Author: Ziming Li
//     Created: 2018-08-31
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_INSPECTOR_TRACE_EVENTS_H
#define BLINKIT_BLINK_INSPECTOR_TRACE_EVENTS_H

#pragma once

#include "platform/TraceEventCommon.h"

namespace blink {

typedef int LayoutInvalidationReasonForTracing;

struct LayoutInvalidationReason {
    enum {
        Unknown,
        SizeChanged,
        AncestorMoved,
        StyleChange,
        DomChanged,
        TextChanged,
        PrintingChanged,
        AttributeChanged,
        ColumnsChanged,
        ChildAnonymousBlockChanged,
        AnonymousBlockChange,
        Fullscreen,
        ChildChanged,
        ListValueChange,
        ImageChanged,
        LineBoxesChanged,
        SliderValueChanged,
        AncestorMarginCollapsing,
        FieldsetChanged,
        TextAutosizing,
        SvgResourceInvalidated,
        FloatDescendantChanged,
        CountersChanged,
        GridChanged,
        MenuWidthChanged,
        RemovedFromLayout,
        AddedToLayout,
        TableChanged,
        PaddingChanged,
        TextControlChanged,
        SvgChanged,
        ScrollbarChanged
    };
};

} // namespace blink

#define TRACE_LAYER_INVALIDATION(...)
#define TRACE_SCHEDULE_STYLE_INVALIDATION(...)
#define TRACE_STYLE_INVALIDATOR_INVALIDATION_SELECTORPART(...)

#endif // BLINKIT_BLINK_INSPECTOR_TRACE_EVENTS_H
