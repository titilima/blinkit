// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_exception_state.h
// Description: DukExceptionState Class
//      Author: Ziming Li
//     Created: 2020-02-08
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_DUK_EXCEPTION_STATE_H
#define BLINKIT_BLINK_DUK_EXCEPTION_STATE_H

#pragma once

#include "duktape/duktape.h"
#include "blinkit/blink/renderer/bindings/core/duk/exception_state.h"

namespace BlinKit {

class DukExceptionState final : public blink::ExceptionState
{
public:
    DukExceptionState(duk_context *ctx);
private:
#if 0 // BKTODO:
    // ExceptionState
    void ThrowException(void) override;
#endif

    duk_context *m_ctx;
};

} // namespace BlinKit

#endif // BLINKIT_BLINK_DUK_EXCEPTION_STATE_H
