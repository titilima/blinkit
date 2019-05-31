// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk.cpp
// Description: Duktape Helpers
//      Author: Ziming Li
//     Created: 2018-08-28
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#include "duk.h"

namespace BlinKit {
namespace Duk {

int DefaultSetter(duk_context *ctx, const char *prop)
{
    duk_push_this(ctx);
    duk_put_prop_string(ctx, -1, prop);
    return 0;
}

int ToErrorCode(duk_context *ctx, duk_idx_t idx)
{
    int code = duk_get_error_code(ctx, idx);
    switch (code)
    {
        case DUK_ERR_NONE:            return BkError::Exception;
        case DUK_ERR_EVAL_ERROR:      return BkError::EvalError;
        case DUK_ERR_RANGE_ERROR:     return BkError::RangeError;
        case DUK_ERR_REFERENCE_ERROR: return BkError::ReferenceError;
        case DUK_ERR_SYNTAX_ERROR:    return BkError::SyntaxError;
        case DUK_ERR_TYPE_ERROR:      return BkError::TypeError;
        case DUK_ERR_URI_ERROR:       return BkError::URIError;
    }

    assert(DUK_ERR_ERROR == code);
    return BkError::UnknownError;
}

std::string ToString(duk_context *ctx, duk_idx_t idx)
{
    size_t l = 0;
    const char *s = duk_to_lstring(ctx, idx, &l);
    return std::string(s, l);
}

} // namespace Duk
} // namespace BlinKit
