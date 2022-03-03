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

namespace blink {

ScriptController::ScriptController(LocalFrame &frame) : m_frame(frame)
{
    ASSERT(false); // BKTODO:
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

std::unique_ptr<ScriptController> ScriptController::Create(LocalFrame &frame)
{
    return zed::wrap_unique(new ScriptController(frame));
}

void ScriptController::executeScriptInMainWorld(const ScriptSourceCode &sourceCode)
{
    ASSERT(false); // BKTODO:
}

void ScriptController::updateDocument(void)
{
    ASSERT(false); // BKTODO:
}

}
