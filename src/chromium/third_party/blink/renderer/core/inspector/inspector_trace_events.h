// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: inspector_trace_events.h
// Description: Placeholders for Inspector
//      Author: Ziming Li
//     Created: 2020-08-28
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_INSPECTOR_TRACE_EVENTS_H
#define BLINKIT_BLINK_INSPECTOR_TRACE_EVENTS_H

#pragma once

#include "third_party/blink/renderer/platform/instrumentation/tracing/trace_event.h"

namespace blink {

typedef int LayoutInvalidationReasonForTracing;
namespace LayoutInvalidationReason {

enum {
    kUnknown,
    kSizeChanged,
    kAncestorMoved,
    kStyleChange,
    kDomChanged,
    kTextChanged,
    kPrintingChanged,
    kAttributeChanged,
    kColumnsChanged,
    kChildAnonymousBlockChanged,
    kAnonymousBlockChange,
    kFullscreen,
    kChildChanged,
    kListValueChange,
    kImageChanged,
    kLineBoxesChanged,
    kSliderValueChanged,
    kAncestorMarginCollapsing,
    kFieldsetChanged,
    kTextAutosizing,
    kSvgResourceInvalidated,
    kFloatDescendantChanged,
    kCountersChanged,
    kGridChanged,
    kMenuOptionsChanged,
    kRemovedFromLayout,
    kAddedToLayout,
    kTableChanged,
    kPaddingChanged,
    kTextControlChanged,
    kSvgChanged,
    kScrollbarChanged
};

} // namespace LayoutInvalidationReason

} // namespace blink

#endif // BLINKIT_BLINK_INSPECTOR_TRACE_EVENTS_H
