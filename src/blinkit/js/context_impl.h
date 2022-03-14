#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: context_impl.h
// Description: ContextImpl Class
//      Author: Ziming Li
//     Created: 2022-03-13
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_CONTEXT_IMPL_H
#define BLINKIT_CONTEXT_IMPL_H

#include "bk_js.h"
#include "third_party/quickjs/quickjs.h"

namespace blink {
class ScriptController;
}

class ContextImpl
{
public:
    ContextImpl(JSContext *ctx, unsigned features = 0);
    virtual ~ContextImpl(void);

    JSContext* GetJSContext(void) const { return m_ctx; }

    int Evaluate(const std::string &code, std::string &ret);
private:
    friend class blink::ScriptController;

    JSContext *m_ctx;

    const bool m_consoleEnabled : 1;
};

#endif // BLINKIT_CONTEXT_IMPL_H
