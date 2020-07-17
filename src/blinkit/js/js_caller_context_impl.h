// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: js_caller_context_impl.h
// Description: JSCallerContextImpl Class
//      Author: Ziming Li
//     Created: 2020-07-14
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_JS_CALLER_CONTEXT_IMPL_H
#define BLINKIT_BLINKIT_JS_CALLER_CONTEXT_IMPL_H

#pragma once

#include <functional>
#include <string_view>
#include "bk_js.h"
#include "blinkit/js/heap_retained.h"
#include "duktape/duktape.h"

class JSCallerContextImpl : public BlinKit::HeapRetainedValue
{
public:
    JSCallerContextImpl(duk_context *ctx, duk_idx_t idx);

    void SetThis(void *thisPtr) { m_thisPtr = thisPtr; }
    int DoPush(const std::function<void(duk_context *)> &worker);
    int Call(BkJSValue *retVal);
private:
    void *m_thisPtr = nullptr;
};

#endif // BLINKIT_BLINKIT_JS_CALLER_CONTEXT_IMPL_H
