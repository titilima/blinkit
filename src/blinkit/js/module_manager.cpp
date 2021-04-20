// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: module_manager.cpp
// Description: ModuleManager Class
//      Author: Ziming Li
//     Created: 2021-04-19
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "module_manager.h"

#include "bkcommon/buffer_impl.hpp"
#include "third_party/blink/renderer/bindings/core/duk/duk.h"

namespace BlinKit {

static const char Modules[] = "modules";
static const char NativeModuleManager[] = "moduleManager";

ModuleManager::ModuleManager(duk_context *ctx, BkModuleLoader loader, void *userData)
    : m_loader(loader), m_userData(userData)
{
    Duk::StackGuard sg(ctx);
    duk_push_heap_stash(ctx);
    duk_push_pointer(ctx, this);
    duk_put_prop_string(ctx, -2, NativeModuleManager);
}

void ModuleManager::Attach(duk_context *ctx)
{
    Duk::StackGuard sg(ctx);
    duk_push_global_stash(ctx);
    duk_push_bare_object(ctx);
    duk_put_prop_string(ctx, -2, Modules);
}

ModuleManager* ModuleManager::From(duk_context *ctx)
{
    Duk::StackGuard sg(ctx);
    duk_push_heap_stash(ctx);
    duk_get_prop_string(ctx, -1, NativeModuleManager);
    return reinterpret_cast<ModuleManager *>(duk_get_pointer(ctx, -1));
}

bool ModuleManager::Load(duk_context *ctx, const char *name)
{
    duk_idx_t modulesIdx;

    duk_push_global_stash(ctx);
    duk_get_prop_string(ctx, -1, Modules);
    ASSERT(duk_is_object(ctx, -1));
    modulesIdx = duk_get_top_index(ctx);

    if (duk_get_prop_string(ctx, -1, name))
        return true;

    std::string code;
    if (!m_loader(name, BufferImpl::Wrap(code), m_userData))
        return false;

    std::string eval("var module = {};");
    eval.append(code);
    duk_peval_lstring(ctx, eval.data(), eval.length());
    if (duk_is_error(ctx, -1))
        return false;

    static const char exports[] = "module.exports";
    duk_eval_lstring(ctx, exports, std::size(exports) - 1);
    duk_put_prop_string(ctx, modulesIdx, name);

    duk_get_prop_string(ctx, modulesIdx, name);
    return true;
}

} // namespace BlinKit
