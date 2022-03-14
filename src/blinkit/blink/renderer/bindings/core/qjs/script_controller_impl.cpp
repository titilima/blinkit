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
#include "blinkit/blink/renderer/core/loader/frame_loader_client.h"
#include "blinkit/blink/renderer/core/frame/LocalFrame.h"
#include "blinkit/js/context_impl.h"

namespace blink {

ScriptController::Prototypes::Prototypes(void)
{
    for (size_t i = 0; i < std::size(prototypes); ++i)
        prototypes[i] = JS_UNINITIALIZED;
}

void ScriptController::Prototypes::Cleanup(JSContext *ctx)
{
    for (const auto &it : elements)
        JS_FreeValue(ctx, it.second);
    elements.clear();
    for (size_t i = 0; i < std::size(prototypes); ++i)
    {
        JS_FreeValue(ctx, prototypes[i]);
        prototypes[i] = JS_UNINITIALIZED;
    }
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
    using namespace qjs;

    if (nullptr == m_ctx)
    {
        ContextImpl *ctx = m_frame.loader().client()->RequireScriptContext();

        std::swap(m_ctx, ctx->m_ctx);
        JS_SetContextOpaque(m_ctx, this);

        JSValue global = JS_GetGlobalObject(m_ctx);

        if (!ctx->m_consoleEnabled)
            AddConsole(m_ctx, global);
        OnContextCreated(m_ctx, global, m_prototypes);

        JSValue window = m_prototypes.prototypes[PROTO_Window];
        ASSERT(JS_IsObject(window));
        JS_SetPrototype(m_ctx, global, window);

        JS_FreeValue(m_ctx, global);

        delete ctx;
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

    JSValue prototype = m_prototypes.elements.end() != it
        ? it->second
        : m_prototypes.prototypes[qjs::PROTO_Element];
    ASSERT(JS_IsObject(prototype));
    return ReturnImpl(m_ctx, element, prototype);
}

JSValue ScriptController::ReturnImpl(JSContext *ctx, ScriptWrappable *nativeObject, JSValueConst prototype)
{
    ASSERT(nullptr != nativeObject);
    if (JS_UNINITIALIZED == nativeObject->m_scriptObject)
        ScriptWrappable::NewObject(nativeObject, ctx, prototype);
    return JS_DupValue(ctx, nativeObject->m_scriptObject);
}

JSValue ScriptController::ReturnNode(Node *node)
{
    if (nullptr == node)
        return JS_NULL;

    if (node->isDocumentNode())
    {
        JSValue prototype = m_prototypes.prototypes[qjs::PROTO_Document];
        ASSERT(JS_IsObject(prototype));
        return ReturnImpl(m_ctx, node, prototype);
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
