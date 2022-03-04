#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: qjs_bindings.h
// Description: Bindings for QuickJS
//      Author: Ziming Li
//     Created: 2022-03-04
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_QJS_BINDINGS_H
#define BLINKIT_QJS_BINDINGS_H

#include "third_party/quickjs/quickjs.h"

namespace BlinKit {
namespace qjs {

void AddConsole(JSContext *ctx, JSValue global);

} // namespace qjs
} // namespace BlinKit

#endif // BLINKIT_QJS_BINDINGS_H
