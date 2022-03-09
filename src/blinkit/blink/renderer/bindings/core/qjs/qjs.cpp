// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: qjs.cpp
// Description: QuickJS Helpers
//      Author: Ziming Li
//     Created: 2022-03-04
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#include "./qjs.h"

namespace BlinKit {
namespace qjs {

JSValue ReturnString(JSContext *ctx, const String &s)
{
    CString ret = s.utf8();
    return JS_NewStringLen(ctx, ret.data(), ret.length());
}

AtomicString ToAtomicString(JSContext *ctx, JSValue v)
{
    size_t l = 0;
    const char *ps = JS_ToCStringLen(ctx, &l, v);
    return AtomicString::fromUTF8(ps, l);
}

std::string ToStdString(JSContext *ctx, JSValue v)
{
    size_t l = 0;
    const char *ps = JS_ToCStringLen(ctx, &l, v);
    return std::string(ps, l);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Context::Context(JSContext &ctx) : m_ctx(&ctx)
{
}

JSValue Context::ReturnException(void)
{
    ASSERT(hadException());
    ASSERT(false); // BKTODO:
    return JS_EXCEPTION;
}

bool Context::ToString(std::string &dst, JSValue v)
{
    bool ret = false;

    if (JS_IsString(v))
    {
        dst = ToStdString(m_ctx, v);
        return true;
    }

    JSValue s = JS_ToString(m_ctx, v);
    if (JS_IsString(s))
    {
        dst = ToStdString(m_ctx, s);
        ret = true;
    }
    JS_FreeValue(m_ctx, s);

    return ret;
}

} // namespace qjs
} // namespace BlinKit
