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

Context::Context(JSContext &ctx) : m_ctx(&ctx)
{
}

bool Context::ToString(std::string &dst, JSValue v)
{
    bool ret = false;

    JSValue s = JS_ToString(m_ctx, v);
    if (JS_IsString(s))
    {
        size_t l = 0;
        const char *ps = JS_ToCStringLen(m_ctx, &l, s);
        dst.assign(ps, l);

        ret = true;
    }
    JS_FreeValue(m_ctx, s);

    return ret;
}

} // namespace qjs
} // namespace qjs
