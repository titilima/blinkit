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

#include "blinkit/js/js_value_impl.h"

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

JSValueImpl* JSCalleeContextImpl::GetArgAt(unsigned argIndex) const
{
    if (argIndex < m_args.size())
        return m_args.at(argIndex).get();
    return nullptr;
}

duk_ret_t JSCalleeContextImpl::Return(void)
{
    ASSERT(false); // BKTODO:
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" {

BKEXPORT unsigned BkGetArgCount(BkJSCalleeContext context)
{
    return context->ArgCount();
}

BKEXPORT int BkGetArgType(BkJSCalleeContext context, unsigned argIndex)
{
    JSValueImpl *arg = context->GetArgAt(argIndex);
    return nullptr != arg ? arg->GetType() : BK_VT_ERROR;
}

BKEXPORT BkJSObject BkGetThis(BkJSCalleeContext context)
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

} // extern "C"
