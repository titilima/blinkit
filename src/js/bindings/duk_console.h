// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_console.h
// Description: Bindings for Console
//      Author: Ziming Li
//     Created: 2019-05-20
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_JS_DUK_CONSOLE_H
#define BLINKIT_JS_DUK_CONSOLE_H

#pragma once

#include "meta_data.h"

namespace BlinKit {

class PrototypeManager;

class DukConsole final
{
public:
    static const char ProtoName[];

    static void RegisterPrototype(duk_context *ctx, PrototypeManager &protos);
private:
    static MetaData ClassMetaData;
};

} // namespace BlinKit

#endif // BLINKIT_JS_DUK_CONSOLE_H
