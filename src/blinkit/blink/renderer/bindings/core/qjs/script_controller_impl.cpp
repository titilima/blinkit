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

ScriptController::~ScriptController(void) = default;

void ScriptController::clearForClose(void)
{
    ASSERT(false); // BKTODO:
}

void ScriptController::clearWindowProxy(void)
{
    ASSERT(false); // BKTODO:
}

void ScriptController::EnsureContext(void)
{
    if (nullptr != m_ctx)
        return;

    m_ctx = JS_NewContext(g_runtime);

    JSValue global = JS_GetGlobalObject(m_ctx);

    using namespace qjs;
    AddConsole(m_ctx, global);
    OnContextCreated(m_ctx, global);

    JS_FreeValue(m_ctx, global);
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
