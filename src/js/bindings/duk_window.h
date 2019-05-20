// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_window.h
// Description: Bindings for Window
//      Author: Ziming Li
//     Created: 2019-05-19
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_JS_DUK_WINDOW_H
#define BLINKIT_JS_DUK_WINDOW_H

#pragma once

#include "meta_data.h"

namespace BlinKit {

class PrototypeManager;

class DukWindow final
{
public:
    static const char ProtoName[];

#ifndef BLINKIT_CRAWLER_ONLY
    static void RegisterPrototype(duk_context *ctx, PrototypeManager &protos);
#endif
    static void RegisterPrototypeForCrawler(duk_context *ctx, PrototypeManager &protos);
private:
    static MetaData ClassMetaData;
};

} // namespace BlinKit

#endif // BLINKIT_JS_DUK_WINDOW_H
