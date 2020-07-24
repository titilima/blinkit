// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: js_callee_context_impl.h
// Description: JSCalleeContextImpl Class
//      Author: Ziming Li
//     Created: 2020-07-23
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_JS_CALLEE_CONTEXT_IMPL_H
#define BLINKIT_BLINKIT_JS_CALLEE_CONTEXT_IMPL_H

#pragma once

#include <string_view>
#include "bk_js.h"
#include "duktape/duktape.h"

class JSCalleeContextImpl
{
public:
    JSCalleeContextImpl(duk_context *ctx, int argc);
    ~JSCalleeContextImpl(void);

    unsigned ArgCount(void) const { return m_args.size(); }
    JSValueImpl* GetArgAt(unsigned argIndex) const;

    JSObjectImpl* GetThis(void) const { return m_thisObject; }
    void SetThis(JSObjectImpl *thisObject) { m_thisObject = thisObject; }

    duk_ret_t Return(void);

    int ReturnBoolean(bool b);
    int ReturnNumber(double d);
    int ReturnString(const std::string_view &s);
private:
    duk_context *m_ctx;
    std::vector<std::unique_ptr<JSValueImpl>> m_args;
    JSObjectImpl *m_thisObject = nullptr;
    std::unique_ptr<JSValueImpl> m_retVal;
};

#endif // BLINKIT_BLINKIT_JS_CALLEE_CONTEXT_IMPL_H
