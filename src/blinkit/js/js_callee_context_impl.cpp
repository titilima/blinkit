// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: js_callee_context_impl.cpp
// Description: JSCalleeContextImpl Class
//      Author: Ziming Li
//     Created: 2020-07-23
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "js_callee_context_impl.h"

#include "base/strings/string_piece.h"
#include "blinkit/js/js_value_impl.h"

using namespace BlinKit;

class Evaluator final : public JSValueImpl
{
public:
    Evaluator(const std::string_view &code) : m_code(code) {}
private:
    int GetType(void) const override
    {
        NOTREACHED();
        return BK_VT_ERROR;
    }
    std::string GetAsString(void) const override
    {
        NOTREACHED();
        return std::string();
    }
    void PushTo(duk_context *ctx) const override
    {
        duk_eval_lstring(ctx, m_code.data(), m_code.length());
    }

    const std::string m_code;
};

JSCalleeContextImpl::JSCalleeContextImpl(duk_context *ctx, int argc) : m_ctx(ctx)
{
    if (0 == argc)
        return;

    m_args.reserve(argc);
    for (int i = 0; i < argc; ++i)
    {
        std::unique_ptr<JSValueImpl> arg(JSValueImpl::Create(ctx, i));
        m_args.push_back(std::move(arg));
    }
}

JSCalleeContextImpl::~JSCalleeContextImpl(void) = default;

int JSCalleeContextImpl::EvaluateAndReturn(const std::string_view &s)
{
    if (m_retVal)
        return BK_ERR_FORBIDDEN;
    m_retVal = std::make_unique<Evaluator>(s);
    return BK_ERR_SUCCESS;
}

JSValueImpl* JSCalleeContextImpl::GetArgAt(unsigned argIndex) const
{
    if (argIndex < m_args.size())
        return m_args.at(argIndex).get();
    return nullptr;
}

duk_ret_t JSCalleeContextImpl::Return(void)
{
    if (!m_retVal)
        return 0;

    m_retVal->PushTo(m_ctx);
    return 1;
}

int JSCalleeContextImpl::ReturnBoolean(bool b)
{
    if (m_retVal)
        return BK_ERR_FORBIDDEN;
    m_retVal = std::make_unique<JSSimpleValue>(b);
    return BK_ERR_SUCCESS;
}

int JSCalleeContextImpl::ReturnNumber(double d)
{
    if (m_retVal)
        return BK_ERR_FORBIDDEN;
    m_retVal = std::make_unique<JSSimpleValue>(d);
    return BK_ERR_SUCCESS;
}

int JSCalleeContextImpl::ReturnString(const std::string_view &s)
{
    if (m_retVal)
        return BK_ERR_FORBIDDEN;
    m_retVal = std::make_unique<JSStringValue>(s.data(), s.length());
    return BK_ERR_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" {

BKEXPORT int BKAPI BkEvaluateAndReturn(BkJSCalleeContext context, const char *code, size_t length)
{
    return context->EvaluateAndReturn(base::WrapStringView(code, length));
}

BKEXPORT unsigned BKAPI BkGetArgCount(BkJSCalleeContext context)
{
    return context->ArgCount();
}

BKEXPORT int BKAPI BkGetArgType(BkJSCalleeContext context, unsigned argIndex)
{
    JSValueImpl *arg = context->GetArgAt(argIndex);
    return nullptr != arg ? arg->GetType() : BK_VT_ERROR;
}

BKEXPORT BkJSObject BKAPI BkGetThis(BkJSCalleeContext context)
{
    return context->GetThis();
}

BKEXPORT int BKAPI BkGetArgAsBoolean(BkJSCalleeContext context, unsigned argIndex, bool_t *dst)
{
    JSValueImpl *arg = context->GetArgAt(argIndex);
    return nullptr != arg ? arg->GetAsBoolean(dst) : BK_ERR_RANGE;
}

BKEXPORT int BKAPI BkGetArgAsInteger(BkJSCalleeContext context, unsigned argIndex, int *dst)
{
    JSValueImpl *arg = context->GetArgAt(argIndex);
    return nullptr != arg ? arg->GetAsInteger(dst) : BK_ERR_RANGE;
}

BKEXPORT int BKAPI BkGetArgAsNumber(BkJSCalleeContext context, unsigned argIndex, double *dst)
{
    JSValueImpl *arg = context->GetArgAt(argIndex);
    return nullptr != arg ? arg->GetAsNumber(dst) : BK_ERR_RANGE;
}

BKEXPORT int BKAPI BkGetArgAsString(BkJSCalleeContext context, unsigned argIndex, struct BkBuffer *dst)
{
    JSValueImpl *arg = context->GetArgAt(argIndex);
    if (nullptr == arg)
        return BK_ERR_RANGE;

    std::string s = arg->GetAsString();
    BkSetBufferData(dst, s.data(), s.length());
    return BK_ERR_SUCCESS;
}

BKEXPORT BkJSObject BKAPI BkGetArgAsObject(BkJSCalleeContext context, unsigned argIndex)
{
    JSValueImpl *arg = context->GetArgAt(argIndex);
    return nullptr != arg ? BkValueToObject(arg) : nullptr;
}

BKEXPORT int BKAPI BkReturnBoolean(BkJSCalleeContext context, bool_t retVal)
{
    return context->ReturnBoolean(retVal);
}

BKEXPORT int BKAPI BkReturnNumber(BkJSCalleeContext context, double retVal)
{
    return context->ReturnNumber(retVal);
}

BKEXPORT int BKAPI BkReturnString(BkJSCalleeContext context, const char *retVal, size_t length)
{
    return context->ReturnString(base::WrapStringView(retVal, length));
}

} // extern "C"
