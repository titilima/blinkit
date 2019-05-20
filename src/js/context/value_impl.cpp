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
