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

#include "core/frame/LocalFrame.h"
#include "platform/weborigin/KURL.h"

#include "context/duk_context.h"
#include "public/script_source_code.h"

using namespace BlinKit;

namespace blink {

ScriptController::ScriptController(LocalFrame *frame) : m_frame(*frame)
{
    // Nothing
}

ScriptController::~ScriptController(void)
{
    // Nothing, just for std::unique_ptr
}

int ScriptController::AccessCrawlerMember(const char *name, BkCallback &callback)
{
    assert(m_frame.IsCrawlerFrame());
    return m_context->AccessCrawlerMember(name, callback);
}

int ScriptController::CallCrawler(const char *method, BkCallback *callback)
{
    assert(m_frame.IsCrawlerFrame());
    return m_context->CallCrawler(method, callback);
}

int ScriptController::CallFunction(const char *name, BkCallback *callback)
{
    if (!m_context)
        return BkError::NotFound;
    return m_context->CallFunction(name, callback);
}

bool ScriptController::canExecuteScripts(ReasonForCallingCanExecuteScripts)
{
    bool scriptEnabled = true;
    if (m_frame.IsCrawlerFrame())
    {
        scriptEnabled = false;
        const auto callback = [&scriptEnabled](const BkValue &prop)
        {
            scriptEnabled = prop.GetAsBoolean();
        };
        m_context->GetCrawlerProperty("scriptEnabled", callback);
    }
    return scriptEnabled;
}

void ScriptController::clearForClose(void)
{
    if (m_context)
        m_context->Reset();
}

void ScriptController::clearScriptObjects(void)
{
    // Nothing to do, this method was for NP objects.
}

void ScriptController::clearWindowProxy(void)
{
    if (m_context)
        m_context->Reset();
}

std::tuple<int, std::string> ScriptController::CreateCrawlerObject(const char *script, size_t length)
{
    assert(m_frame.IsCrawlerFrame());
    return EnsureContext().CreateCrawlerObject(script, length);
}

void ScriptController::disableEval(const String &errorMessage)
{
    assert(false); // Not reached!
}

void ScriptController::enableEval(void)
{
    // Nothing to do, eval is always enabled in BlinKit.
}

int ScriptController::Eval(const char *code, size_t length, BkCallback *callback)
{
    RefPtrWillBeRawPtr<LocalFrame> protect(&m_frame);
    if (m_frame.loader().stateMachine()->isDisplayingInitialEmptyDocument())
        m_frame.loader().didAccessInitialDocument();
    return EnsureContext().Eval(code, length, callback);
}

DukContext& ScriptController::EnsureContext(void)
{
    if (!m_context)
        m_context = std::make_unique<DukContext>(m_frame);
    return *m_context;
}

bool ScriptController::executeScriptIfJavaScriptURL(const KURL &url)
{
    assert(!protocolIsJavaScript(url));
    return false;
}

void ScriptController::executeScriptInMainWorld(const ScriptSourceCode &sourceCode, AccessControlStatus accessControlStatus, double *compilationFinishTime)
{
    RefPtrWillBeRawPtr<LocalFrame> protect(&m_frame);
    if (m_frame.loader().stateMachine()->isDisplayingInitialEmptyDocument())
        m_frame.loader().didAccessInitialDocument();

    CString code = sourceCode.source().utf8();
    std::string fileName = sourceCode.FileName();
    EnsureContext().Eval(code.data(), code.length(), nullptr, fileName.c_str());

    if (nullptr != compilationFinishTime)
        *compilationFinishTime = WTF::monotonicallyIncreasingTime();
}

void ScriptController::GetCrawlerProperty(const char *name, const std::function<void(const BkValue &)> &callback)
{
    if (m_context)
        m_context->GetCrawlerProperty(name, callback);
}

void ScriptController::namedItemAdded(HTMLDocument *, const AtomicString &)
{
    assert(false); // BKTODO:
}

void ScriptController::namedItemRemoved(HTMLDocument *, const AtomicString &)
{
    assert(false); // BKTODO:
}

int ScriptController::RegisterFunction(const char *name, BkCallback &functionImpl)
{
    return EnsureContext().RegisterFunction(name, functionImpl);
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
