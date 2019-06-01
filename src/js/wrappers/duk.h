// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk.h
// Description: Duktape Helpers
//      Author: Ziming Li
//     Created: 2018-08-28
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_JS_DUK_H
#define BLINKIT_JS_DUK_H

#pragma once

namespace blink {
class ScriptWrappable;
}

namespace WTF {
class AtomicString;
class String;
}

namespace BlinKit {
namespace Duk {

class StackKeeper {
public:
    StackKeeper(duk_context *ctx, int delta = 0) : m_ctx(ctx), m_top(duk_get_top(ctx)), m_delta(delta) {}
    ~StackKeeper(void) { duk_set_top(m_ctx, m_top - m_delta); }
protected:
    duk_context *m_ctx;
private:
    const int m_top, m_delta;
};

void BindNativeThis(duk_context *ctx, blink::ScriptWrappable *nativeThis, duk_idx_t idx = -1);
blink::ScriptWrappable* GetNativeThis(duk_context *ctx, duk_idx_t idx = -1);

int DefaultSetter(duk_context *ctx, const char *prop);

int PushString(duk_context *ctx, const WTF::String &s);
int PushString(duk_context *ctx, const WTF::AtomicString &s);

int ToErrorCode(duk_context *ctx, duk_idx_t idx = -1);
std::string ToString(duk_context *ctx, duk_idx_t idx = -1);
WTF::String ToWTFString(duk_context *ctx, duk_idx_t idx = -1);

} // namespace Duk
} // namespace BlinKit

#endif // BLINKIT_JS_DUK_H
