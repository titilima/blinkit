// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: script_wrappable.h
// Description: ScriptWrappable Class
//      Author: Ziming Li
//     Created: 2019-01-15
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_SCRIPT_WRAPPABLE_H
#define BLINKIT_BLINK_SCRIPT_WRAPPABLE_H

#pragma once

#include "platform/heap/Heap.h"
#include "wtf/Noncopyable.h"

namespace blink {

class ScriptWrappable
{
    WTF_MAKE_NONCOPYABLE(ScriptWrappable);
public:
    virtual ~ScriptWrappable(void) = default;
protected:
    ScriptWrappable(void) = default;
};

} // namespace blink

/**
 * Just placeholders
 */

#define DECLARE_WRAPPERTYPEINFO()
#define DEFINE_WRAPPERTYPEINFO()

#endif // BLINKIT_BLINK_SCRIPT_WRAPPABLE_H
