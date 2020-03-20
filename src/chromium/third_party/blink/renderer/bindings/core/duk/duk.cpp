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

const char* PushString(duk_context *ctx, const WTF::String &s)
{
    const std::string s2 = s.StdUtf8();
    return duk_push_lstring(ctx, s2.data(), s2.length());
}

} // namespace Duk
} // namespace BlinKit
