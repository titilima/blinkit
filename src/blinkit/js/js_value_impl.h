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
#include "blinkit/js/heap_retained.h"

class JSValueImpl
{
public:
    static JSValueImpl* Create(duk_context *ctx, duk_idx_t idx);
    virtual ~JSValueImpl(void) = default;

    virtual int GetType(void) const = 0;
    virtual int GetAsBoolean(bool_t *dst) const { return BK_ERR_TYPE; }
    virtual int GetAsInteger(int *dst) const { return BK_ERR_TYPE; }
    virtual int GetAsNumber(double *dst) const { return BK_ERR_TYPE; }
    virtual std::string GetAsString(void) const = 0;
    virtual void PushTo(duk_context *ctx) const = 0;
protected:
    JSValueImpl(void) = default;
};

class JSErrorImpl final : public JSValueImpl
{
public:
    JSErrorImpl(duk_context *ctx, duk_idx_t idx);
    JSErrorImpl(int code);

    int GetCode(void) const { return m_code; }
    int GetInfo(const char *field, BkBuffer *dst) const;
private:
    int GetType(void) const override { return BK_VT_ERROR; }
    std::string GetAsString(void) const override;
    void PushTo(duk_context *ctx) const override { NOTREACHED(); }

    int m_code;
    std::unordered_map<std::string, std::string> m_infoMap;
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
    void PushTo(duk_context *ctx) const override;

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
    void PushTo(duk_context *ctx) const override { duk_push_lstring(ctx, m_stringVal.data(), m_stringVal.length()); }

    std::string m_stringVal;
};

class JSHeapValue : public JSValueImpl, public BlinKit::HeapRetained
{
public:
    ~JSHeapValue(void) override;

    std::string ToJSON(void) const;
    void PushTo(duk_context *ctx) const final { HeapRetained::PushTo(ctx); }
protected:
    JSHeapValue(duk_context *ctx, duk_idx_t idx);

    duk_context *m_ctx;
private:
    std::string GetAsString(void) const final;
};

} // namespace BlinKit

class JSArrayImpl final : public BlinKit::JSHeapValue
{
public:
    JSArrayImpl(duk_context *ctx, duk_idx_t idx) : JSHeapValue(ctx, idx) {}

    unsigned GetSize(void) const;
    JSValueImpl* GetMember(unsigned index);
private:
    int GetType(void) const override { return BK_VT_ARRAY; }
};

class JSObjectImpl final : public BlinKit::JSHeapValue
{
public:
    JSObjectImpl(duk_context *ctx, duk_idx_t idx) : JSHeapValue(ctx, idx) {}

    JSValueImpl* GetMember(const char *name);
private:
    int GetType(void) const override { return BK_VT_OBJECT; }
};

#endif // BLINKIT_BLINKIT_JS_VALUE_IMPL_H
