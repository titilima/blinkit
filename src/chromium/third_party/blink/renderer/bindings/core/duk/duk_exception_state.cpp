// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_exception_state.cpp
// Description: DukExceptionState Class
//      Author: Ziming Li
//     Created: 2020-02-08
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "duk_exception_state.h"

namespace BlinKit {

DukExceptionState::DukExceptionState(duk_context *ctx) : m_ctx(ctx)
{
}

} // namespace BlinKit
