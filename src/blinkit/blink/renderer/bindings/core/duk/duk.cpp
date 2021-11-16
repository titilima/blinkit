// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk.cpp
// Description: Duktape Wrappers
//      Author: Ziming Li
//     Created: 2020-03-01
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "./duk.h"

namespace BlinKit {
namespace Duk {

void CollectStringArgs(duk_context *ctx, std::vector<std::string> &dst)
{
    const duk_idx_t n = duk_get_top(ctx);
    for (duk_idx_t i = 0; i < n; ++i)
    {
        size_t l = 0;
        const char *s = duk_safe_to_lstring(ctx, i, &l);
        if (nullptr == s)
            continue;

        dst.emplace_back(s, l);
    }
}

const char* PushString(duk_context *ctx, const std::string &s)
{
    return duk_push_lstring(ctx, s.data(), s.length());
}

const char* PushString(duk_context *ctx, const WTF::String &s)
{
    return PushString(ctx, s.stdUtf8());
}

bool TryToArrayIndex(duk_context *ctx, duk_idx_t idx, duk_uarridx_t &dst)
{
    if (!duk_is_number(ctx, idx))
    {
        if (!duk_is_string(ctx, idx))
            return false;

        size_t l = 0;
        const char *s = duk_get_lstring(ctx, idx, &l);
        for (size_t i = 0; i < l; ++i)
        {
            if (!isdigit(s[i]))
                return false;
        }
    }
    dst = duk_to_uint(ctx, idx);
    return true;
}

} // namespace Duk
} // namespace BlinKit
