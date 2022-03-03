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

namespace blink {

static JSRuntime *g_runtime = nullptr;

ScriptWrappable::~ScriptWrappable(void)
{
    if (0 != m_contextObject)
        JS_FreeValueRT(g_runtime, m_contextObject);
}

void ScriptWrappable::Initialize(JSRuntime *runtime)
{
    ASSERT(nullptr == g_runtime);
    g_runtime = runtime;
}

} // namespace blink
