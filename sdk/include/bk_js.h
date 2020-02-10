// -------------------------------------------------
// BlinKit SDK
// -------------------------------------------------
//   File Name: bk_js.h
//      Author: Ziming Li
//     Created: 2020-01-21
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_SDK_JS_H
#define BLINKIT_SDK_JS_H

#pragma once

#include "bk_def.h"

#ifdef __cplusplus
extern "C" {
#endif

BK_DECLARE_HANDLE(BkJSValue, JSValueImpl);
BK_DECLARE_HANDLE(BkJSError, JSErrorImpl);

BKEXPORT void BKAPI BkReleaseValue(BkJSValue val);

enum BkValueType {
    BK_VT_ERROR     = -1,
    BK_VT_UNDEFINED =  0,
    BK_VT_NULL,
    BK_VT_BOOLEAN,
    BK_VT_NUMBER,
    BK_VT_STRING,
    BK_VT_ARRAY,
    BK_VT_OBJECT
};

BKEXPORT int BKAPI BkGetValueType(BkJSValue val);

BKEXPORT int BKAPI BkGetBooleanValue(BkJSValue val, bool_t *dst);
BKEXPORT int BKAPI BkGetIntegerValue(BkJSValue val, int *dst);
BKEXPORT int BKAPI BkGetNumberValue(BkJSValue val, double *dst);
BKEXPORT int BKAPI BkGetValueAsString(BkJSValue val, struct BkBuffer *dst);
BKEXPORT BkJSError BKAPI BkValueToError(BkJSValue val);

enum BkEvalFlags {
    BK_EVAL_DEFAULT             =   0,
    BK_EVAL_IGNORE_RETURN_VALUE = 0x1
};

BKEXPORT BkJSValue BKAPI BkJSEvaluate(BkJSContext context, const char *code, unsigned flags);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // BLINKIT_SDK_JS_H
