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

#include <cfloat>

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
#ifndef NDEBUG
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
        default:
            NOTREACHED();
    }

    return std::string();
}

void JSSimpleValue::PushTo(duk_context *ctx) const
{
    switch (m_type)
    {
        case BK_VT_UNDEFINED:
            duk_push_undefined(ctx);
            break;
        case BK_VT_NULL:
            duk_push_null(ctx);
            break;
        case BK_VT_BOOLEAN:
            duk_push_boolean(ctx, m_booleanVal);
            break;
        case BK_VT_NUMBER:
            if (CanBeTreatedAsInteger())
                duk_push_int(ctx, static_cast<int>(m_numberVal));
            else
                duk_push_number(ctx, m_numberVal);
            break;
        default:
            NOTREACHED();
    }
}

JSHeapValue::JSHeapValue(duk_context *ctx, duk_idx_t idx) : HeapRetained(DUK_HIDDEN_SYMBOL("val")), m_ctx(ctx)
{
    HeapRetained::Retain(m_ctx, idx);
}

std::string JSHeapValue::GetAsString(void) const
{
    std::string ret;

    HeapRetained::PushTo(m_ctx);

    size_t l = 0;
    const char *s = duk_safe_to_lstring(m_ctx, -1, &l);
    ret.assign(s, l);

    duk_pop(m_ctx);
    return ret;
}

JSHeapValue::~JSHeapValue(void)
{
    HeapRetained::Release(m_ctx);
}

std::string JSHeapValue::ToJSON(void) const
{
    std::string ret;

    HeapRetained::PushTo(m_ctx);
    duk_json_encode(m_ctx, -1);

    size_t l = 0;
    const char *s = duk_get_lstring(m_ctx, -1, &l);
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

BKEXPORT int BKAPI BkGetErrorCode(BkJSError e)
{
    return e->GetCode();
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

BKEXPORT int BKAPI BkObjectToJSON(BkJSObject o, struct BkBuffer *dst)
{
    std::string s = o->ToJSON();
    BkSetBufferData(dst, s.data(), s.length());
    return BK_ERR_SUCCESS;
}

BKEXPORT void BKAPI BkReleaseValue(BkJSValue val)
{
    val->Release();
}

BKEXPORT BkJSError BKAPI BkValueToError(BkJSValue val)
{
    return BK_VT_ERROR == val->GetType() ? static_cast<JSErrorImpl *>(val) : nullptr;
}

BKEXPORT BkJSObject BKAPI BkValueToObject(BkJSValue val)
{
    return BK_VT_OBJECT == val->GetType() ? static_cast<JSObjectImpl *>(val) : nullptr;
}

} // extern "C"
