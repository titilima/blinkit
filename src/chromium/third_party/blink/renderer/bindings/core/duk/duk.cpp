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

#include "duk.h"

namespace BlinKit {
namespace Duk {

const char* PushString(duk_context *ctx, const std::string &s)
{
    return duk_push_lstring(ctx, s.data(), s.length());
}

const char* PushString(duk_context *ctx, const WTF::String &s)
{
    return PushString(ctx, s.StdUtf8());
}

} // namespace Duk
} // namespace BlinKit
