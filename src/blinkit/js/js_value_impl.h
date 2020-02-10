// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: js_value_impl.h
// Description: JSValueImpl Class
//      Author: Ziming Li
//     Created: 2020-02-09
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_JS_VALUE_IMPL_H
#define BLINKIT_BLINKIT_JS_VALUE_IMPL_H

#pragma once

#include "bk_js.h"
#include "duktape/duktape.h"

class JSValueImpl
{
public:
    static JSValueImpl* Create(duk_context *ctx, duk_idx_t idx);

    void Release(void) { delete this; }
    virtual int GetType(void) const = 0;
    virtual int GetAsBoolean(bool_t *dst) const { return BK_ERR_TYPE; }
    virtual int GetAsInteger(int *dst) const { return BK_ERR_TYPE; }
    virtual int GetAsNumber(double *dst) const { return BK_ERR_TYPE; }
    virtual std::string GetAsString(void) const = 0;
protected:
    virtual ~JSValueImpl(void) = default;
};

class JSErrorImpl final : public JSValueImpl
{
public:
    JSErrorImpl(duk_context *ctx, duk_idx_t idx);
private:
    static std::string Extract(duk_context *ctx, duk_idx_t idx, const char *field);

    int GetType(void) const override { return BK_VT_ERROR; }
    std::string GetAsString(void) const override;

    int m_code;
    std::string m_name, m_message, m_fileName;
#ifdef _DEBUG
    std::string m_stack;
#endif
    int m_lineNumber = 0;
};

namespace BlinKit {

class JSSimpleValue final : public JSValueImpl
{
public:
    JSSimpleValue(BkValueType type) : m_type(type) {}
    JSSimpleValue(bool booleanVal) : m_type(BK_VT_BOOLEAN)
    {
        m_booleanVal = booleanVal;
    }
    JSSimpleValue(double numberVal) : m_type(BK_VT_NUMBER)
    {
        m_numberVal = numberVal;
    }
private:
    bool CanBeTreatedAsInteger(void) const;

    int GetType(void) const override { return m_type; }
    int GetAsBoolean(bool_t *dst) const override;
    int GetAsInteger(int *dst) const override;
    int GetAsNumber(double *dst) const override;
    std::string GetAsString(void) const override;

    BkValueType m_type;
    union {
        bool m_booleanVal;
        double m_numberVal;
    };
};

class JSStringValue final : public JSValueImpl
{
public:
    JSStringValue(const char *s, size_t length) : m_stringVal(s, length) {}
private:
    int GetType(void) const override { return BK_VT_STRING; }
    std::string GetAsString(void) const override { return m_stringVal; }

    std::string m_stringVal;
};

class JSHeapValue : public JSValueImpl
{
public:
    ~JSHeapValue(void) override;
protected:
    JSHeapValue(duk_context *ctx, duk_idx_t idx);

    duk_context *m_ctx;
    void *m_heapPtr;
private:
    std::string GetAsString(void) const final;

    std::string m_key;
};

} // namespace BlinKit

class JSArrayImpl final : public BlinKit::JSHeapValue
{
public:
    JSArrayImpl(duk_context *ctx, duk_idx_t idx) : JSHeapValue(ctx, idx) {}
private:
    int GetType(void) const override { return BK_VT_ARRAY; }
};

class JSObjectImpl final : public BlinKit::JSHeapValue
{
public:
    JSObjectImpl(duk_context *ctx, duk_idx_t idx) : JSHeapValue(ctx, idx) {}
private:
    int GetType(void) const override { return BK_VT_OBJECT; }
};

#endif // BLINKIT_BLINKIT_JS_VALUE_IMPL_H
