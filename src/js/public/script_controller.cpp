// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: script_controller.cpp
// Description: ScriptController Class
//      Author: Ziming Li
//     Created: 2019-03-07
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "public/script_controller.h"

#include "platform/weborigin/KURL.h"

#include "context/duk_context.h"

namespace blink {

ScriptController::ScriptController(LocalFrame *frame)
{
    // Nothing
}

ScriptController::~ScriptController(void)
{
    // Nothing, just for std::unique_ptr
}

bool ScriptController::canExecuteScripts(ReasonForCallingCanExecuteScripts)
{
    return true; // Scripts are always allowed in BlinKit.
}

void ScriptController::clearForClose(void)
{
    m_context.reset();
}

void ScriptController::clearScriptObjects(void)
{
    // Nothing to do, this method was for NP objects.
}

void ScriptController::clearWindowProxy(void)
{
    m_context.reset();
}

void ScriptController::disableEval(const String &errorMessage)
{
    assert(false); // Not reached!
}

void ScriptController::enableEval(void)
{
    // Nothing to do, eval is always enabled in BlinKit.
}

bool ScriptController::executeScriptIfJavaScriptURL(const KURL &url)
{
    assert(!protocolIsJavaScript(url));
    return false;
}

void ScriptController::executeScriptInMainWorld(const ScriptSourceCode &sourceCode, AccessControlStatus accessControlStatus, double *compilationFinishTime)
{
    assert(false); // BKTODO:
}

void ScriptController::namedItemAdded(HTMLDocument *, const AtomicString &)
{
    assert(false); // BKTODO:
}

void ScriptController::namedItemRemoved(HTMLDocument *, const AtomicString &)
{
    assert(false); // BKTODO:
}

void ScriptController::updateDocument(void)
{
    if (m_context)
        m_context->Reset();
}

void ScriptController::updateSecurityOrigin(SecurityOrigin *)
{
    assert(false); // BKTODO:
}

} // namespace blink
