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
#include "third_party/blink/renderer/platform/wtf/text/atomic_string.h"

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
    return WTF::AtomicString::FromUTF8(s, l);
}

template <>
inline WTF::String To<WTF::String>(duk_context *ctx, duk_idx_t idx)
{
    size_t l = 0;
    const char *s = duk_to_lstring(ctx, idx, &l);
    return WTF::String::FromUTF8(s, l);
}

const char* PushString(duk_context *ctx, const WTF::String &s);

} // namespace Duk
} // namespace BlinKit

#endif // BLINKIT_BLINK_DUK_H
