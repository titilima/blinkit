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

#include "context_impl.h"

#include "base/strings/string_piece.h"
#include "blinkit/js/function_manager.h"
#include "blinkit/js/js_caller_context_impl.h"
#include "blinkit/js/js_value_impl.h"
#include "blinkit/js/module_manager.h"
#include "blinkit/js/simple_context.h"
#include "third_party/blink/renderer/bindings/core/duk/duk.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_global.h"
#include "third_party/blink/renderer/bindings/core/duk/script_controller.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/frame/local_frame_client.h"

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
        return BK_ERR_TYPE;

    m_moduleManager = std::make_unique<ModuleManager>(m_ctx, loader, userData);
    m_moduleManager->Attach(m_ctx);
    return BK_ERR_SUCCESS;
}

duk_context* ContextImpl::EnsureDukSession(void)
{
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

    Duk::StackGuard sg(m_ctx);

    JSCallerContextImpl *ret = nullptr;
    do {
        void *thisPtr = nullptr;
        if (BK_CTX_GLOBAL == callContext)
        {
            duk_push_global_object(m_ctx);
        }
        else
        {
            ASSERT(false); // BKTODO:
#if 0
            JSObjectImpl *contextObject = GetContextObject(callContext);
            if (nullptr == contextObject)
                break;

            contextObject->PushTo(m_ctx);
            thisPtr = duk_get_heapptr(m_ctx, -1);
#endif
        }

        ASSERT(!sg.IsNotChanged());
        if (!duk_is_object(m_ctx, -1))
            break;
        if (!duk_get_prop_string(m_ctx, -1, functionName))
            break;
        if (!duk_is_callable(m_ctx, -1))
            break;

        ret = new JSCallerContextImpl(m_ctx, -1);
        if (nullptr != thisPtr)
            ret->SetThis(thisPtr);
    } while (false);
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
    if (BK_CTX_USER_OBJECT == memberContext)
    {
        ScriptController *ctx = ScriptController::From(m_ctx);
        if (nullptr == ctx || !ctx->GetFrame().Client()->IsCrawler())
            return BK_ERR_FORBIDDEN;
    }

    if (!m_functionManager)
        m_functionManager = std::make_unique<FunctionManager>();
    ASSERT(false); // TODO:
    return 0;
#if 0
    return m_functionManager->Register(memberContext, functionName, impl, userData);
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" {

BKEXPORT BkJSContext BKAPI BkCreateJSContext(void)
{
    return new BlinKit::SimpleContext;
}

BKEXPORT int BKAPI BkDestroyJSContext(BkJSContext context)
{
    if (context->QueryDestroy())
    {
        delete context;
        return BK_ERR_SUCCESS;
    }
    else
    {
        return BK_ERR_FORBIDDEN;
    }
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
    context->Eval(base::WrapStringView(code, len), callback);

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
