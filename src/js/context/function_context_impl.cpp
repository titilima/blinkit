// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: function_context_impl.cpp
// Description: FunctionContextImpl Class
//      Author: Ziming Li
//     Created: 2019-05-21
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "function_context_impl.h"

#include "context/value_impl.h"

namespace BlinKit {

FunctionContextImpl::FunctionContextImpl(duk_context *ctx, const std::string &name) : m_ctx(ctx), m_name(name)
{
    int argc = duk_get_top(ctx);
    for (int i = 0; i < argc; ++i)
        m_args.push_back(ValueImpl(ctx, i));
}

FunctionContextImpl::~FunctionContextImpl(void)
{
    // Nothing
}

const BkValue* BKAPI FunctionContextImpl::ArgAt(size_t i) const
{
    if (m_args.size() <= i)
        return nullptr;

    const ValueImpl &arg = m_args.at(i);
    return &arg;
}

size_t BKAPI FunctionContextImpl::ArgCount(void) const
{
    return m_args.size();
}

int BKAPI FunctionContextImpl::ReturnAsBoolean(bool b)
{
    if (m_hasReturnValue)
        return BkError::Forbidden;
    duk_push_boolean(m_ctx, b);
    m_hasReturnValue = true;
    return BkError::Success;
}

int BKAPI FunctionContextImpl::ReturnAsJSON(const char *json, size_t length)
{
    if (m_hasReturnValue)
        return BkError::Forbidden;

    if (0 == length)
        duk_push_string(m_ctx, json);
    else
        duk_push_lstring(m_ctx, json, length);
    duk_json_decode(m_ctx, -1);
    m_hasReturnValue = true;
    return BkError::Success;
}

} // namespace BlinKit
