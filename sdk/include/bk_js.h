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

/**
 * Value
 */

BK_DECLARE_HANDLE(BkJSValue, JSValueImpl);
BK_DECLARE_HANDLE(BkJSError, JSErrorImpl);
BK_DECLARE_HANDLE(BkJSObject, JSObjectImpl);

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
BKEXPORT int BKAPI BkGetErrorCode(BkJSError e);

BKEXPORT BkJSObject BKAPI BkValueToObject(BkJSValue val);
BKEXPORT int BKAPI BkObjectToJSON(BkJSObject o, struct BkBuffer *dst);

/**
 * ArgList & Writers
 */

BK_DECLARE_HANDLE(BkJSArrayWriter, JSArrayWriterImpl);
BK_DECLARE_HANDLE(BkJSObjectWriter, JSObjectWriterImpl);

typedef BkJSArrayWriter BkJSArgList;

/**
 * Context
 */

BKEXPORT BkJSValue BKAPI BkGetUserObject(BkJSContext context);

BKEXPORT int BKAPI BkEvaluate(BkJSContext context, const char *code, BkJSValue *retVal);

BKEXPORT int BKAPI BkCall(BkJSContext context, BkJSObject scope, const char *func, BkJSArgList argList, BkJSValue *retVal);

enum BkConsoleMessageType {
    BK_CONSOLE_LOG = 0,
    BK_CONSOLE_WARN,
    BK_CONSOLE_ERROR
};

#ifdef __cplusplus
} // extern "C"
#endif

#endif // BLINKIT_SDK_JS_H
