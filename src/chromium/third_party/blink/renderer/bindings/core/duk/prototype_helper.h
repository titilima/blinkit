// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: prototype_helper.h
// Description: PrototypeHelper Class
//      Author: Ziming Li
//     Created: 2020-01-22
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_PROTOTYPE_HELPER_H
#define BLINKIT_BLINK_PROTOTYPE_HELPER_H

#pragma once

#include <iterator> // for std::size
#include "duktape/duktape.h"

namespace blink {
class ScriptWrappable;
}

namespace BlinKit {

class PrototypeEntry final
{
    friend class PrototypeHelper;
public:
    ~PrototypeEntry(void);

    void SetFinalizer(duk_c_function finalizer);

    struct Property {
        const char *name;
        duk_c_function getter;
        duk_c_function setter; // Could be nullptr
    };
    void Add(const Property *properties, size_t count, duk_uint_t extraFlags = 0);

    struct Method {
        const char *name;
        duk_c_function impl;
        duk_idx_t argc;
    };
    void Add(const Method *methods, size_t count, duk_uint_t extraFlags = 0);
private:
    PrototypeEntry(duk_context *ctx, const char *name);

    duk_context *m_ctx;
    const char *m_name;
#ifdef _DEBUG
    void *m_heapPtr;
#endif
};

class PrototypeHelper final
{
public:
    PrototypeHelper(duk_context *ctx);
    ~PrototypeHelper(void);

    typedef void (*Worker)(PrototypeEntry &);
    void Register(const char *protoName, Worker worker);

    static duk_idx_t CreateScriptObject(duk_context *ctx, const char *protoName, blink::ScriptWrappable &nativeObject);
private:
    duk_context *m_ctx;
#ifdef _DEBUG
    void *m_heapPtr;
#endif
};

} // namespace BlinKit

#endif // BLINKIT_BLINK_PROTOTYPE_HELPER_H
