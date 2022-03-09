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
#include "blinkit/blink/renderer/bindings/core/script_wrappable.h"
#include "blinkit/blink/renderer/bindings/core/qjs/qjs_bindings.h"
#include "blinkit/blink/renderer/core/dom/element.h"
#include "blinkit/blink/renderer/core/frame/LocalFrame.h"
#include "blinkit/js/runtime.h"

namespace blink {

void ScriptController::Prototypes::Cleanup(JSContext *ctx)
{
    JS_FreeValue(ctx, genericElement);
    for (const auto &it : elements)
        JS_FreeValue(ctx, it.second);
    elements.clear();

    JS_FreeValue(ctx, document);
    
    JS_FreeValue(ctx, containerNode);
    JS_FreeValue(ctx, node);
    JS_FreeValue(ctx, eventTarget);

    JS_FreeValue(ctx, window);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
        m_prototypes.Cleanup(m_ctx);

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
        JS_SetContextOpaque(m_ctx, this);

        JSValue global = JS_GetGlobalObject(m_ctx);

        using namespace qjs;
        AddConsole(m_ctx, global);
        OnContextCreated(m_ctx, global, m_prototypes);

        ASSERT(JS_IsObject(m_prototypes.window));
        JS_SetPrototype(m_ctx, global, m_prototypes.window);

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

ScriptController* ScriptController::From(JSContext *ctx)
{
    ScriptController *ret = reinterpret_cast<ScriptController *>(JS_GetContextOpaque(ctx));
    ASSERT(ret->m_ctx == ctx);
    return ret;
}

LocalDOMWindow* ScriptController::GetWindow(void)
{
    return m_frame.localDOMWindow();
}

JSValue ScriptController::ReturnElement(Element *element)
{
    if (nullptr == element)
        return JS_NULL;
    return ReturnElementImpl(element);
}

JSValue ScriptController::ReturnElementImpl(Element *element)
{
    auto it = m_prototypes.elements.find(element->localName());

    JSValue prototype = m_prototypes.elements.end() != it ? it->second : m_prototypes.genericElement;
    return ReturnImpl(m_ctx, element, prototype);
}

JSValue ScriptController::ReturnImpl(JSContext *ctx, ScriptWrappable *nativeObject, JSValueConst prototype)
{
    ASSERT(nullptr != nativeObject);
    if (JS_UNINITIALIZED != nativeObject->m_scriptObject)
        return JS_DupValue(ctx, nativeObject->m_scriptObject);
    return ScriptWrappable::NewObject(ctx, prototype, nativeObject);
}

JSValue ScriptController::ReturnNode(Node *node)
{
    if (nullptr == node)
        return JS_NULL;

    if (node->isDocumentNode())
    {
        ASSERT(JS_IsObject(m_prototypes.document));
        return ReturnImpl(m_ctx, node, m_prototypes.document);
    }

    if (node->isElementNode())
        return ReturnElementImpl(toElement(node));

    ASSERT(false); // BKTODO:
    return JS_NULL;
}

void ScriptController::updateDocument(void)
{
    if (nullptr == m_ctx)
        return;
    ASSERT(false); // BKTODO:
}

}
