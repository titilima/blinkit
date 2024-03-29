// -------------------------------------------------
// BlinKit SDK
// -------------------------------------------------
//   File Name: bk_js.h
//      Author: Ziming Li
//     Created: 2022-03-07
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_SDK_JS_H
#define BLINKIT_SDK_JS_H

#pragma once

#include "bk_def.h"

#ifdef __cplusplus
extern "C" {
#endif

enum BkContextFeature {
    BK_CTX_CONSOLE = 0x1
};

BKEXPORT BkContext BKAPI BkCreateContext(unsigned features);
BKEXPORT void BKAPI BkReleaseContext(BkContext ctx);

BKEXPORT BkJSContext BKAPI BkGetJSContext(BkContext ctx);

BKEXPORT int BKAPI BkEvaluate(BkJSContext jsCtx, const char *code, unsigned len, struct BkBuffer *ret);

#ifdef __cplusplus
}
#endif

#endif // BLINKIT_SDK_JS_H
