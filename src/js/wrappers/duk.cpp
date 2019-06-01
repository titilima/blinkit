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

#include "wtf/text/WTFString.h"

using namespace blink;

namespace BlinKit {

static const char NativeThisKey[] = DUK_HIDDEN_SYMBOL("nativeThis");

namespace Duk {

void BindNativeThis(duk_context *ctx, ScriptWrappable *nativeThis, duk_idx_t idx)
{
    assert(DUK_TYPE_OBJECT == duk_get_type(ctx, idx));

    idx = duk_normalize_index(ctx, idx);

    duk_push_pointer(ctx, nativeThis);
    duk_put_prop_string(ctx, idx, NativeThisKey);
}

int DefaultSetter(duk_context *ctx, const char *prop)
{
    duk_push_this(ctx);
    duk_put_prop_string(ctx, -1, prop);
    return 0;
}

ScriptWrappable* GetNativeThis(duk_context *ctx, duk_idx_t idx)
{
    assert(DUK_TYPE_OBJECT == duk_get_type(ctx, idx));

    StackKeeper sk(ctx);
    if (!duk_get_prop_string(ctx, idx, NativeThisKey))
        return nullptr;
    return reinterpret_cast<ScriptWrappable *>(duk_to_pointer(ctx, -1));
}

int PushString(duk_context *ctx, const String &s)
{
    const std::string utf8 = s.to_string();
    duk_push_lstring(ctx, utf8.data(), utf8.length());
    return 1;
}

int PushString(duk_context *ctx, const AtomicString &s)
{
    const std::string utf8 = s.to_string();
    duk_push_lstring(ctx, utf8.data(), utf8.length());
    return 1;
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

String ToWTFString(duk_context *ctx, duk_idx_t idx)
{
    size_t l = 0;
    const char *s = duk_to_lstring(ctx, idx, &l);
    return String::fromUTF8(s, l);
}

} // namespace Duk
} // namespace BlinKit
