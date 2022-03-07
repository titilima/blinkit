// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: script_controller_impl.cpp
// Description: ScriptController Class
//      Author: Ziming Li
//     Created: 2022-03-02
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#include "../script_controller.h"

#include "blinkit/blink/renderer/bindings/core/script_source_code.h"
#include "blinkit/blink/renderer/bindings/core/qjs/qjs_bindings.h"
#include "blinkit/js/runtime.h"

namespace blink {

ScriptController::ScriptController(LocalFrame &frame) : m_frame(frame)
{
}

ScriptController::~ScriptController(void)
{
    ASSERT(nullptr == m_ctx);
}

void ScriptController::clearForClose(void)
{
    if (nullptr != m_ctx)
    {
        JS_FreeContext(m_ctx);
        m_ctx = nullptr;
    }
}

void ScriptController::clearWindowProxy(void)
{
    ASSERT(false); // BKTODO:
}

JSContext* ScriptController::EnsureContext(void)
{
    if (nullptr == m_ctx)
    {
        m_ctx = JS_NewContext(g_runtime);

        JSValue global = JS_GetGlobalObject(m_ctx);

        using namespace qjs;
        AddConsole(m_ctx, global);
        OnContextCreated(m_ctx, global);

        JS_FreeValue(m_ctx, global);
    }
    return m_ctx;
}

void ScriptController::executeScriptInMainWorld(const ScriptSourceCode &sourceCode)
{
    EnsureContext();

    std::string input = sourceCode.source().stdUtf8();
    JSValue ret = JS_Eval(m_ctx, input.c_str(), input.length(), sourceCode.FileName().c_str(), JS_EVAL_TYPE_GLOBAL);
    // TODO: Process error.
    JS_FreeValue(m_ctx, ret);
}

void ScriptController::updateDocument(void)
{
    if (nullptr == m_ctx)
        return;
    ASSERT(false); // BKTODO:
}

}
