// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_global.h
// Description: DukGlobal Class
//      Author: Ziming Li
//     Created: 2020-07-30
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_DUK_GLOBAL_H
#define BLINKIT_BLINK_DUK_GLOBAL_H

#pragma once

#include "duktape/duktape.h"
#include "third_party/blink/renderer/platform/wtf/allocator.h"

namespace BlinKit {

class DukGlobal
{
    STATIC_ONLY(DukGlobal);
public:
    static void Attach(duk_context *ctx, duk_idx_t dst);
private:
    static void AddMethod(duk_context *ctx, duk_idx_t dst, const char *name, duk_c_function impl, duk_idx_t nargs = DUK_VARARGS);
    static void AddProperty(duk_context *ctx, duk_idx_t dst, const char *name, duk_c_function getter, duk_c_function setter = nullptr);
};

} // namespace BlinKit

#endif // BLINKIT_BLINK_DUK_GLOBAL_H
