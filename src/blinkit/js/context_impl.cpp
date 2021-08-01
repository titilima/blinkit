// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: context_impl.cpp
// Description: ContextImpl Class
//      Author: Ziming Li
//     Created: 2020-01-21
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "./context_impl.h"

#include "blinkit/blink/renderer/bindings/core/duk/duk.h"
#include "blinkit/blink/renderer/bindings/core/duk/duk_global.h"
#include "blinkit/blink/renderer/bindings/core/duk/script_controller.h"
#include "blinkit/js/function_manager.h"
#include "blinkit/js/js_caller_context_impl.h"
#include "blinkit/js/js_value_impl.h"
#include "blinkit/js/module_manager.h"
#include "blinkit/js/simple_context.h"
#if 0 // BKTODO:
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/frame/local_frame_client.h"
#endif

using namespace blink;
using namespace BlinKit;

#ifndef NDEBUG
static void FatalCallback(void *, const char *msg)
{
    NOTREACHED();
}
#endif

ContextImpl::ContextImpl(void) = default;

ContextImpl::~ContextImpl(void)
{
    DestroyDukSession();
}

void ContextImpl::Attach(duk_context *ctx, duk_idx_t globalStashIndex)
{
    if (m_moduleManager)
        m_moduleManager->Attach(m_ctx, globalStashIndex);
}

void ContextImpl::DefaultConsoleOutput(int, const char *msg)
{
    BkLog("%s", msg);
}

void ContextImpl::DestroyDukSession(void)
{
    if (nullptr != m_ctx)
    {
        Detach(m_ctx);
        duk_destroy_heap(m_ctx);
        m_ctx = nullptr;
    }
}

int ContextImpl::EnableModules(BkModuleLoader loader, void *userData)
{
    if (m_moduleManager)
        return BK_ERR_FORBIDDEN;
    if (nullptr == loader)
        return BK_ERR_PARAM;

    m_moduleManager = std::make_unique<ModuleManager>(loader, userData);
    if (nullptr != m_ctx)
    {
        Duk::StackGuard _(m_ctx);
        duk_push_global_stash(m_ctx);
        m_moduleManager->Attach(m_ctx, duk_normalize_index(m_ctx, -1));
    }
    return BK_ERR_SUCCESS;
}

duk_context* ContextImpl::EnsureDukSession(void)
{
    if (IsDukSessionDirty())
        DestroyDukSession();

    if (nullptr == m_ctx)
    {
#ifdef NDEBUG
        m_ctx = duk_create_heap_default();
#else
        m_ctx = duk_create_heap(nullptr, nullptr, nullptr, nullptr, FatalCallback);
#endif

        Duk::StackGuard _(m_ctx);

        duk_idx_t globalIndex;
        duk_push_global_object(m_ctx);
        globalIndex = duk_normalize_index(m_ctx, -1);
        DukGlobal::Attach(m_ctx, globalIndex);

        duk_idx_t globalStashIndex;
        duk_push_global_stash(m_ctx);
        globalStashIndex = duk_normalize_index(m_ctx, -1);

        Attach(m_ctx, globalStashIndex);

        if (m_functionManager)
        {
            FunctionManager::Indices indices = { 0 };
            indices.globalObjectIndex = globalIndex;

            void *userObject = GetUserObject();
            if (nullptr != userObject)
            {
                duk_push_heapptr(m_ctx, userObject);
                indices.userObjectIndex = duk_normalize_index(m_ctx, -1);
            }

            m_functionManager->FlushAll(m_ctx, indices, userObject);
        }
    }
    return m_ctx;
}

void ContextImpl::Eval(const std::string_view &code, const Callback &callback, const char *fileName)
{
    Duk::StackGuard sg(m_ctx);

    int r;
    if (nullptr == fileName || '\0' == *fileName)
    {
        r = duk_pcompile_lstring(m_ctx, 0, code.data(), code.length());
    }
    else
    {
        duk_push_string(m_ctx, fileName);
        r = duk_pcompile_lstring_filename(m_ctx, 0, code.data(), code.length());
    }

    if (DUK_EXEC_SUCCESS == r)
        r = duk_pcall(m_ctx, 0);
    callback(m_ctx);
}

BkJSCallerContext ContextImpl::PrepareFunctionCall(int callContext, const char *functionName)
{
    EnsureDukSession();

    Duk::StackGuard _(m_ctx);

    void *thisPtr = nullptr;
    switch (callContext)
    {
        case BK_CTX_GLOBAL:
            duk_push_global_object(m_ctx);
            break;
        case BK_CTX_USER_OBJECT:
            thisPtr = GetUserObject();
            if (nullptr == thisPtr)
                return nullptr;
            duk_push_heapptr(m_ctx, thisPtr);
            break;
        default:
            NOTREACHED();
            return nullptr;
    }

    if (!duk_is_object(m_ctx, -1))
    {
        ASSERT(duk_is_object(m_ctx, -1));
        return nullptr;
    }

    if (!duk_get_prop_string(m_ctx, -1, functionName))
        return nullptr;

    if (!duk_is_callable(m_ctx, -1))
    {
        NOTREACHED();
        return nullptr;
    }

    JSCallerContextImpl *ret = new JSCallerContextImpl(m_ctx, -1);
    if (nullptr != thisPtr)
        ret->SetThis(thisPtr);
    return ret;
}

BkJSCallerContext ContextImpl::PrepareScriptFunction(const char *code)
{
    EnsureDukSession();

    Duk::StackGuard _(m_ctx);
    int r = duk_peval_string(m_ctx, code);
    if (DUK_EXEC_SUCCESS != r || !duk_is_function(m_ctx, -1))
        return nullptr;
    return new JSCallerContextImpl(m_ctx, -1);
}

int ContextImpl::RegisterFunction(int memberContext, const char *functionName, BkFunctionImpl impl, void *userData)
{
    FunctionManager::FunctionData data = { memberContext, impl, userData };

    if (!m_functionManager)
        m_functionManager = std::make_unique<FunctionManager>();

    std::string name(functionName);
    if (nullptr == m_ctx)
        return m_functionManager->Register(name, data);

    Duk::StackGuard _(m_ctx);

    void *userObject = GetUserObject();
    switch (memberContext)
    {
        case BK_CTX_GLOBAL:
            duk_push_global_object(m_ctx);
            break;
        case BK_CTX_USER_OBJECT:
            if (nullptr == userObject)
                return BK_ERR_FORBIDDEN;
            duk_push_heapptr(m_ctx, userObject);
            break;
        default:
            NOTREACHED();
            return BK_ERR_PARAM;
    }

    return m_functionManager->Register(m_ctx, duk_normalize_index(m_ctx, -1), name, data, userObject);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" {

BKEXPORT BkJSContext BKAPI BkCreateJSContext(void)
{
    return new BlinKit::SimpleContext;
}

BKEXPORT int BKAPI BkDestroyJSContext(BkJSContext context)
{
    if (context->IsScriptController())
        return BK_ERR_FORBIDDEN;
    delete context;
    return BK_ERR_SUCCESS;
}

BKEXPORT int BKAPI BkEnableModules(BkJSContext context, int, BkModuleLoader loader, void *userData)
{
    return context->EnableModules(loader, userData);
}

BKEXPORT int BKAPI BkEvaluate(BkJSContext context, const char *code, size_t len, BkJSValue *retVal)
{
    std::unique_ptr<JSValueImpl> ret;
    const auto callback = [&ret](duk_context *ctx)
    {
        ret.reset(JSValueImpl::Create(ctx, -1));
    };
    context->Eval(std::string_view(code, len), callback);

    int r = ret->GetType() == BK_VT_ERROR
        ? static_cast<JSErrorImpl *>(ret.get())->GetCode()
        : BK_ERR_SUCCESS;
    if (nullptr != retVal)
        *retVal = ret.release();
    return r;
}

BKEXPORT BkJSCallerContext BKAPI BkPrepareFunctionCall(BkJSContext context, int callContext, const char *functionName)
{
    return context->PrepareFunctionCall(callContext, functionName);
}

BKEXPORT BkJSCallerContext BKAPI BkPrepareScriptFunction(BkJSContext context, const char *code)
{
    return context->PrepareScriptFunction(code);
}

BKEXPORT int BKAPI BkRegisterFunction(BkJSContext context, int memberContext, const char *functionName, BkFunctionImpl impl, void *userData)
{
    return context->RegisterFunction(memberContext, functionName, impl, userData);
}

} // extern "C"
