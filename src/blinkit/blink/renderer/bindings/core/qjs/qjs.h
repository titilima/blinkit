#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: qjs.h
// Description: QuickJS Helpers
//      Author: Ziming Li
//     Created: 2022-03-04
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_QJS_H
#define BLINKIT_QJS_H

#include "blinkit/blink/renderer/wtf/Allocator.h"
#include "third_party/quickjs/quickjs.h"

namespace BlinKit {
namespace qjs {

class Context
{
    STACK_ALLOCATED()
public:
    Context(JSContext &ctx);

    bool ToString(std::string &dst, JSValue v);
private:
    std::string ToStringNotChecked(JSValue v);

    JSContext *m_ctx;
};

} // namespace qjs
} // namespace qjs

#endif // BLINKIT_QJS_H
