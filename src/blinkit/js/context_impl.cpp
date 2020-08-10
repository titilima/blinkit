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

#include "blinkit/js/browser_context.h"
#include "blinkit/js/function_manager.h"
#include "blinkit/js/js_caller_context_impl.h"
#include "blinkit/js/js_value_impl.h"
#include "blinkit/js/simple_context.h"
#include "third_party/blink/renderer/bindings/core/duk/duk.h"
#include "third_party/blink/renderer/platform/bindings/gc_pool.h"

using namespace BlinKit;

ContextImpl::ContextImpl(void)
    : m_ctx(duk_create_heap_default())
{
}

ContextImpl::~ContextImpl(void)
{
    GCPool pool(m_ctx);
    duk_destroy_heap(m_ctx);
    pool.DetachContext();
}

void ContextImpl::Eval(const std::string_view code, const Callback &callback, const char *fileName)
{
    GCPool gcPool(m_ctx);
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
            JSObjectImpl *contextObject = GetContextObject(callContext);
            if (nullptr == contextObject)
                break;

            contextObject->PushTo(m_ctx);
            thisPtr = duk_get_heapptr(m_ctx, -1);
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

void ContextImpl::RegisterFunctions(void)
{
    if (m_functionManager)
        m_functionManager->RegisterTo(m_ctx);
}

int ContextImpl::RegisterFunction(int memberContext, const char *functionName, BkFunctionImpl impl, void *userData)
{
    if (!m_functionManager)
        m_functionManager = std::make_unique<FunctionManager>(*this);
    return m_functionManager->Register(m_ctx, memberContext, functionName, impl, userData);
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

BKEXPORT int BKAPI BkEvaluate(BkJSContext context, const char *code, BkJSValue *retVal)
{
    std::unique_ptr<JSValueImpl> ret;
    const auto callback = [&ret](duk_context *ctx)
    {
        ret.reset(JSValueImpl::Create(ctx, -1));
    };
    context->Eval(code, callback);

    int r = ret->GetType() == BK_VT_ERROR
        ? static_cast<JSErrorImpl *>(ret.get())->GetCode()
        : BK_ERR_SUCCESS;
    if (nullptr != retVal)
        *retVal = ret.release();
    return r;
}

BKEXPORT BkJSObject BKAPI BkGetUserObject(BkJSContext context)
{
    return context->GetContextObject(BK_CTX_USER_OBJECT);
}

BKEXPORT BkJSCallerContext BKAPI BkPrepareFunctionCall(BkJSContext context, int callContext, const char *functionName)
{
    return context->PrepareFunctionCall(callContext, functionName);
}

BKEXPORT int BKAPI BkRegisterFunction(BkJSContext context, int memberContext, const char *functionName, BkFunctionImpl impl, void *userData)
{
    return context->RegisterFunction(memberContext, functionName, impl, userData);
}

} // extern "C"
