// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: prototype_manager.h
// Description: PrototypeManager Class
//      Author: Ziming Li
//     Created: 2019-05-19
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_JS_PROTOTYPE_MANAGER_H
#define BLINKIT_JS_PROTOTYPE_MANAGER_H

#pragma once

#include <functional>
#include <unordered_map>
#include "wtf/StdLibExtras.h"

namespace BlinKit {

class PrototypeEntry final
{
    friend class PrototypeManager;
public:
    void Add(const char *name, int value);
    void AddObject(const char *name);

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
    PrototypeEntry(duk_context *ctx, duk_idx_t idx);

    duk_context *m_ctx;
    const duk_idx_t m_idx;
};

class PrototypeManager final
{
public:
    PrototypeManager(duk_context *ctx);

    bool CreateObject(duk_context *ctx, const char *protoName);

    void BeginRegisterTransaction(duk_context *ctx);
    bool Register(duk_context *ctx, const char *protoName, const std::function<void(PrototypeEntry &)> &worker);
    void EndRegisterTransaction(duk_context *ctx);
private:
    void *m_protosPtr;
    std::unordered_map<std::string, void *> m_protos;
};

} // namespace BlinKit

#endif // BLINKIT_JS_PROTOTYPE_MANAGER_H
