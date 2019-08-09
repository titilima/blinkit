// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: value_impl.cpp
// Description: ValueImpl Class
//      Author: Ziming Li
//     Created: 2019-05-18
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "value_impl.h"

#include "wrappers/duk.h"

namespace BlinKit {

ValueImpl::ValueImpl(duk_context *ctx, duk_idx_t idx) : m_ctx(ctx), m_idx(duk_normalize_index(ctx, idx))
{
    // Nothing
}

bool BKAPI ValueImpl::GetAsBoolean(void) const
{
    Duk::StackKeeper sk(m_ctx);

    duk_idx_t idx = m_idx;
    if (!duk_is_boolean(m_ctx, m_idx))
    {
        duk_dup(m_ctx, m_idx);
        idx = -1;
    }

    return duk_to_boolean(m_ctx, idx);
}

int BKAPI ValueImpl::GetAsJSON(BkBuffer &dst) const
{
    Duk::StackKeeper sk(m_ctx);

    duk_dup(m_ctx, m_idx);
    duk_json_encode(m_ctx, -1);

    size_t l = 0;
    const char *s = duk_get_lstring(m_ctx, -1, &l);
    dst.Assign(s, l);
    return BkError::Success;
}

int BKAPI ValueImpl::GetAsString(BkBuffer &dst) const
{
    Duk::StackKeeper sk(m_ctx);

    duk_idx_t idx = m_idx;
    if (!duk_is_string(m_ctx, m_idx))
    {
        duk_dup(m_ctx, m_idx);
        idx = -1;
    }

    size_t l = 0;
    const char *s = duk_to_lstring(m_ctx, idx, &l);
    dst.Assign(s, l);
    return BkError::Success;
}

BkValue::Type BKAPI ValueImpl::GetType(void) const
{
    if (BkError::Success != m_errorCode)
        return ErrorType;
    if (duk_is_object(m_ctx, m_idx))
        return ObjectType;
    if (duk_is_string(m_ctx, m_idx))
        return StringType;
    if (duk_is_number(m_ctx, m_idx))
        return NumberType;
    if (duk_is_array(m_ctx, m_idx))
        return ArrayType;
    if (duk_is_boolean(m_ctx, m_idx))
        return BooleanType;
    if (duk_is_null(m_ctx, m_idx))
        return NullType;
    if (duk_is_undefined(m_ctx, m_idx))
        return UndefinedType;
    assert(false); // Not reached!
    return UnknownType;
}

void ValueImpl::SetAsErrorType(void)
{
    m_errorCode = Duk::ToErrorCode(m_ctx, m_idx);
#ifdef _DEBUG
    duk_get_prop_string(m_ctx, m_idx, "message");
    BKLOG("ERROR: %s", duk_to_string(m_ctx, -1));
    duk_pop(m_ctx);
#endif
}

} // namespace BlinKit
