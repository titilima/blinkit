// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: js_value_impl.cpp
// Description: JSValueImpl Class
//      Author: Ziming Li
//     Created: 2020-02-09
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "js_value_impl.h"

using namespace BlinKit;

JSValueImpl* JSValueImpl::Create(duk_context *ctx, duk_idx_t idx)
{
    if (duk_is_error(ctx, idx))
        return new JSErrorImpl(ctx, idx);

    switch (duk_get_type(ctx, idx))
    {
        case DUK_TYPE_UNDEFINED:
            return new JSSimpleValue(BK_VT_UNDEFINED);
        case DUK_TYPE_NULL:
            return new JSSimpleValue(BK_VT_NULL);
        case DUK_TYPE_BOOLEAN:
        {
            bool booleanVal = duk_get_boolean(ctx, idx);
            return new JSSimpleValue(booleanVal);
        }
        case DUK_TYPE_NUMBER:
        {
            double numberVal = duk_get_number(ctx, idx);
            return new JSSimpleValue(numberVal);
        }
        case DUK_TYPE_STRING:
        {
            size_t l = 0;
            const char *s = duk_get_lstring(ctx, idx, &l);
            return new JSStringValue(s, l);
        }
        case DUK_TYPE_OBJECT:
        {
            if (duk_is_array(ctx, idx))
                return new JSArrayImpl(ctx, idx);
            else
                return new JSObjectImpl(ctx, idx);
        }
    }

    BKLOG("Unexpected type: %d!", duk_get_type(ctx, idx));
    ASSERT(false); // Not reached!
    return nullptr;
}

JSErrorImpl::JSErrorImpl(duk_context *ctx, duk_idx_t idx)
{
    const duk_idx_t top = duk_get_top(ctx);
    idx = duk_normalize_index(ctx, idx);

    const duk_errcode_t code = duk_get_error_code(ctx, idx);
    switch (code)
    {
        case DUK_ERR_EVAL_ERROR:      m_code = BK_ERR_EVAL;      break;
        case DUK_ERR_RANGE_ERROR:     m_code = BK_ERR_RANGE;     break;
        case DUK_ERR_REFERENCE_ERROR: m_code = BK_ERR_REFERENCE; break;
        case DUK_ERR_SYNTAX_ERROR:    m_code = BK_ERR_SYNTAX;    break;
        case DUK_ERR_TYPE_ERROR:      m_code = BK_ERR_TYPE;      break;
        case DUK_ERR_URI_ERROR:       m_code = BK_ERR_URI;       break;

        default:
            BKLOG("Unexpected error: %d!", code);
            m_code = BK_ERR_UNKNOWN;
    }

    m_name = Extract(ctx, idx, "name");
    m_message = Extract(ctx, idx, "message");
    m_fileName = Extract(ctx, idx, "fileName");
#ifdef _DEBUG
    m_stack = Extract(ctx, idx, "stack");
#endif
    if (duk_get_prop_string(ctx, idx, "lineNumber"))
        m_lineNumber = duk_to_int(ctx, -1);

    duk_set_top(ctx, top);
}

std::string JSErrorImpl::Extract(duk_context *ctx, duk_idx_t idx, const char *field)
{
    std::string ret;
    if (duk_get_prop_string(ctx, idx, field))
    {
        ASSERT(duk_is_string(ctx, -1));

        size_t l = 0;
        const char *s = duk_get_lstring(ctx, -1, &l);
        ret.assign(s, l);
    }
    return ret;
}

std::string JSErrorImpl::GetAsString(void) const
{
    std::string ret(m_name);
    ret.push_back(':');
    ret.push_back(' ');
    ret.append(m_message);
    return ret;
}

namespace BlinKit {

bool JSSimpleValue::CanBeTreatedAsInteger(void) const
{
    ASSERT(BK_VT_NUMBER == m_type);

    int intVal = static_cast<int>(m_numberVal);
    double doubleVal = intVal;
    return abs(m_numberVal - doubleVal) < DBL_EPSILON;
}

int JSSimpleValue::GetAsBoolean(bool_t *dst) const
{
    if (BK_VT_BOOLEAN != m_type)
        return BK_ERR_TYPE;
    *dst = m_booleanVal;
    return BK_ERR_SUCCESS;
}

int JSSimpleValue::GetAsInteger(int *dst) const
{
    if (BK_VT_NUMBER != m_type)
        return BK_ERR_TYPE;
    *dst = static_cast<int>(m_numberVal);
    return BK_ERR_SUCCESS;
}

int JSSimpleValue::GetAsNumber(double *dst) const
{
    if (BK_VT_NUMBER != m_type)
        return BK_ERR_TYPE;
    *dst = m_numberVal;
    return BK_ERR_SUCCESS;
}

std::string JSSimpleValue::GetAsString(void) const
{
    switch (m_type)
    {
        case BK_VT_UNDEFINED:
            return "undefined";
        case BK_VT_NULL:
            return "null";
        case BK_VT_BOOLEAN:
            return m_booleanVal ? "true" : "false";
        case BK_VT_NUMBER:
            if (CanBeTreatedAsInteger())
            {
                int intVal = static_cast<int>(m_numberVal);
                return std::to_string(intVal);
            }
            return std::to_string(m_numberVal);
    }

    ASSERT(false); // Not reached!
    return std::string();
}

JSHeapValue::JSHeapValue(duk_context *ctx, duk_idx_t idx) : m_ctx(ctx), m_heapPtr(duk_get_heapptr(ctx, idx))
{
    char buf[32];
    sprintf(buf, "%x", reinterpret_cast<unsigned int>(this));
    m_key.assign(DUK_HIDDEN_SYMBOL("val"));
    m_key.append(buf);

    duk_push_global_object(m_ctx);
    duk_push_heapptr(m_ctx, m_heapPtr);
    duk_put_prop_lstring(m_ctx, -2, m_key.data(), m_key.length());
    duk_pop(m_ctx);
}

JSHeapValue::~JSHeapValue(void)
{
    duk_push_global_object(m_ctx);
    duk_del_prop_lstring(m_ctx, -1, m_key.data(), m_key.length());
    duk_pop(m_ctx);
}

std::string JSHeapValue::GetAsString(void) const
{
    std::string ret;

    duk_push_heapptr(m_ctx, m_heapPtr);

    size_t l = 0;
    const char *s = duk_safe_to_lstring(m_ctx, -1, &l);
    ret.assign(s, l);

    duk_pop(m_ctx);
    return ret;
}

} // namespace BlinKit

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" {

BKEXPORT int BKAPI BkGetBooleanValue(BkJSValue val, bool_t *dst)
{
    return val->GetAsBoolean(dst);
}

BKEXPORT int BKAPI BkGetIntegerValue(BkJSValue val, int *dst)
{
    return val->GetAsInteger(dst);
}

BKEXPORT int BKAPI BkGetNumberValue(BkJSValue val, double *dst)
{
    return val->GetAsNumber(dst);
}

BKEXPORT int BKAPI BkGetValueAsString(BkJSValue val, struct BkBuffer *dst)
{
    std::string s = val->GetAsString();
    BkSetBufferData(dst, s.data(), s.length());
    return BK_ERR_SUCCESS;
}

BKEXPORT int BKAPI BkGetValueType(BkJSValue val)
{
    return val->GetType();
}

BKEXPORT void BKAPI BkReleaseValue(BkJSValue val)
{
    val->Release();
}

BKEXPORT BkJSError BKAPI BkValueToError(BkJSValue val)
{
    return BK_VT_ERROR == val->GetType() ? static_cast<JSErrorImpl *>(val) : nullptr;
}

} // extern "C"
