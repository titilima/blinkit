// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: caller_context_impl.h
// Description: CallerContextImpl Class
//      Author: Ziming Li
//     Created: 2019-05-18
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_JS_CALLER_CONTEXT_IMPL_H
#define BLINKIT_JS_CALLER_CONTEXT_IMPL_H

#pragma once

#include "wrappers/duk.h"

namespace BlinKit {

class CallerContextImpl final : public BkArgList, private Duk::StackKeeper
{
public:
    CallerContextImpl(duk_context *ctx);

    void SetAsThisCall(void) { m_thisCall = true; }

    int Call(const char *name, BkCallback *callback);
private:
    int BKAPI PushInt(int arg) override;
    int BKAPI PushString(const char *arg, size_t length) override;

    bool m_thisCall = false;
    int m_argc = 0;
};

} // namespace BlinKit

#endif // BLINKIT_JS_CALLER_CONTEXT_IMPL_H
