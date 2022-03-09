#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: qjs.h
// Description: QuickJS Helpers
//      Author: Ziming Li
//     Created: 2022-03-04
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_QJS_H
#define BLINKIT_QJS_H

#include "blinkit/blink/renderer/bindings/core/exception_state.h"
#include "blinkit/blink/renderer/wtf/text/AtomicString.h"
#include "third_party/quickjs/quickjs.h"

namespace BlinKit {
namespace qjs {

WTF::AtomicString ToAtomicString(JSContext *ctx, JSValue v);
std::string ToStdString(JSContext *ctx, JSValue v);

JSValue ReturnString(JSContext *ctx, const WTF::String &s);

class Context final : public ExceptionState
{
    STACK_ALLOCATED()
public:
    Context(JSContext &ctx);

    bool ToString(std::string &dst, JSValue v);

    JSValue ReturnException(void);
private:
    JSContext *m_ctx;
};

} // namespace qjs
} // namespace qjs

#endif // BLINKIT_QJS_H
