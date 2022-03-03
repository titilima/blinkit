// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: script_wrappable_impl.cpp
// Description: ScriptWrappable Class
//      Author: Ziming Li
//     Created: 2022-03-02
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#include "./script_wrappable_impl.h"

#include "blinkit/js/runtime.h"

namespace blink {

ScriptWrappable::~ScriptWrappable(void)
{
    if (0 != m_contextObject)
        JS_FreeValueRT(g_runtime, m_contextObject);
}

} // namespace blink
