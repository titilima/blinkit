// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk.h
// Description: Duktape Wrappers
//      Author: Ziming Li
//     Created: 2020-03-01
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_DUK_H
#define BLINKIT_BLINK_DUK_H

#pragma once

#include "duktape/duktape.h"
#include "blinkit/blink/renderer/wtf/text/AtomicString.h"

namespace BlinKit {
namespace Duk {

template <typename T> T To(duk_context *, duk_idx_t);

template <>
inline std::string To<std::string>(duk_context *ctx, duk_idx_t idx)
{
    size_t l = 0;
    const char *s = duk_to_lstring(ctx, idx, &l);
    return std::string(s, l);
}

template <>
inline WTF::AtomicString To<WTF::AtomicString>(duk_context *ctx, duk_idx_t idx)
{
    size_t l = 0;
    const char *s = duk_to_lstring(ctx, idx, &l);
    return WTF::AtomicString::fromUTF8(s, l);
}

template <>
inline WTF::String To<WTF::String>(duk_context *ctx, duk_idx_t idx)
{
    size_t l = 0;
    const char *s = duk_to_lstring(ctx, idx, &l);
    return WTF::String::fromUTF8(s, l);
}

bool TryToArrayIndex(duk_context *ctx, duk_idx_t idx, duk_uarridx_t &dst);

const char* PushString(duk_context *ctx, const std::string &s);
const char* PushString(duk_context *ctx, const WTF::String &s);

class StackGuard
{
public:
    StackGuard(duk_context *ctx) : m_ctx(ctx), m_top(duk_get_top(ctx)) {}
    ~StackGuard(void) { duk_set_top(m_ctx, m_top); }

    void Keep(int n) { m_top += n; }
    bool IsChangedBy(int n) const { return duk_get_top(m_ctx) - n == m_top; }
    bool IsNotChanged(void) const { return duk_get_top(m_ctx) == m_top; }
private:
    duk_context *m_ctx;
    duk_idx_t m_top;
};

} // namespace Duk
} // namespace BlinKit

#endif // BLINKIT_BLINK_DUK_H
