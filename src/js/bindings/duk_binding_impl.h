// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_binding_impl.h
// Description: Binding Implementations
//      Author: Ziming Li
//     Created: 2019-05-31
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_JS_DUK_BINDING_IMPL_H
#define BLINKIT_JS_DUK_BINDING_IMPL_H

#pragma once

namespace BlinKit {

namespace Crawler {

duk_ret_t NothingToDo(duk_context *ctx);

duk_ret_t ElementAttributeGetter(duk_context *ctx, const char *name);
duk_ret_t ElementAttributeSetter(duk_context *ctx, const char *name);

} // namespace Crawler

} // namespace BlinKit

#endif // BLINKIT_JS_DUK_BINDING_IMPL_H
