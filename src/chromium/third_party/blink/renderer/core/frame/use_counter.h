// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: use_counter.h
// Description: UseCounter Class
//      Author: Ziming Li
//     Created: 2019-10-30
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_USE_COUNTER_H
#define BLINKIT_BLINK_USE_COUNTER_H

#pragma once

#include "third_party/blink/renderer/core/frame/web_feature.h"
#include "third_party/blink/renderer/platform/wtf/allocator.h"

namespace blink {

enum CSSPropertyID;
class Document;
class ExecutionContext;

class UseCounter
{
    STATIC_ONLY(UseCounter);
public:
    static void Count(const Document &, WebFeature) {}
    static void Count(ExecutionContext *, WebFeature) {}
    static void CountAnimatedCSS(const Document &, CSSPropertyID) {}
};

} // namespace blink

#endif // BLINKIT_BLINK_USE_COUNTER_H
