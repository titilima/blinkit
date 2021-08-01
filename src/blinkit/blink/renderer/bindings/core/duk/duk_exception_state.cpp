// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: duk_exception_state.cpp
// Description: DukExceptionState Class
//      Author: Ziming Li
//     Created: 2020-02-08
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "./duk_exception_state.h"

using namespace blink;

namespace BlinKit {

DukExceptionState::DukExceptionState(duk_context *ctx) : m_ctx(ctx)
{
}

#if 0 // BKTODO:
void DukExceptionState::ThrowException(void)
{
    DOMExceptionCode code = static_cast<DOMExceptionCode>(m_exceptionCode);
    switch (code)
    {
        case DOMExceptionCode::kSyntaxError:
            duk_syntax_error(m_ctx, "%s", m_message.c_str());
            break;
        default:
            ASSERT(false); // BKTODO:
    }
}
#endif

} // namespace BlinKit
