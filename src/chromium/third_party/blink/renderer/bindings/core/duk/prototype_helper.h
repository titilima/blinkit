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
#include <string>
#include <unordered_map>
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

    void SetFinalizer(duk_c_function finalizer) { m_finalizer = finalizer; }

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

    void Add(const char *name, duk_int_t type) { m_simpleMembers[name] = type; }

    static const char NameKey[];
private:
    PrototypeEntry(duk_context *ctx, const char *name);

    duk_context *m_ctx;
    const char *m_name;
#ifndef NDEBUG
    const int m_top;
#endif

    duk_c_function m_finalizer = nullptr;
    struct PropertyData {
        duk_c_function getter;
        duk_c_function setter; // Could be nullptr
        duk_uint_t flags;
    };
    std::unordered_map<std::string, PropertyData> m_properties;
    struct MethodData {
        duk_c_function impl;
        duk_idx_t argc;
        duk_uint_t flags;
    };
    std::unordered_map<std::string, MethodData> m_methods;
    std::unordered_map<std::string, duk_int_t> m_simpleMembers;
};

class PrototypeHelper final
{
public:
    PrototypeHelper(duk_context *ctx, duk_idx_t globalStashIndex);
    ~PrototypeHelper(void);

    typedef void (*Worker)(PrototypeEntry &);
    void Register(const char *protoName, Worker worker);

    static bool AttachToScriptObject(duk_context *ctx, duk_idx_t idx, const char *protoName);
    static duk_idx_t CreateScriptObject(duk_context *ctx, const char *protoName, blink::ScriptWrappable *nativeObject);
private:
    duk_context *m_ctx;
    const duk_idx_t m_globalStashIndex;
#ifndef NDEBUG
    const duk_idx_t m_prototypesIndex;
#endif
};

} // namespace BlinKit

#endif // BLINKIT_BLINK_PROTOTYPE_HELPER_H
