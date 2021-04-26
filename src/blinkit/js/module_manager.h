#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: module_manager.h
// Description: ModuleManager Class
//      Author: Ziming Li
//     Created: 2021-04-19
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_MODULE_MANAGER_H
#define BLINKIT_BLINKIT_MODULE_MANAGER_H

#include "bk_js.h"
#include "duktape/duktape.h"

namespace BlinKit {

class ModuleManager
{
public:
    ModuleManager(BkModuleLoader loader, void *userData);
    static ModuleManager* From(duk_context *ctx);

    void Attach(duk_context *ctx, duk_idx_t globalStashIndex);
    bool Load(duk_context *ctx, const char *name);
private:
    BkModuleLoader m_loader;
    void *m_userData;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_MODULE_MANAGER_H
