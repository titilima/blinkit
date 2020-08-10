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
BK_DECLARE_HANDLE(BkJSArray, JSArrayImpl);
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
BKEXPORT int BKAPI BkGetErrorInfo(BkJSError e, const char *field, struct BkBuffer *dst);

BKEXPORT BkJSArray BKAPI BkValueToArray(BkJSValue val);
BKEXPORT int BKAPI BkArrayToJSON(BkJSArray a, struct BkBuffer *dst);
BKEXPORT unsigned BKAPI BkArrayGetSize(BkJSArray a);
BKEXPORT BkJSValue BKAPI BkArrayGetMember(BkJSArray a, unsigned index);

BKEXPORT BkJSObject BKAPI BkValueToObject(BkJSValue val);
BKEXPORT int BKAPI BkObjectToJSON(BkJSObject o, struct BkBuffer *dst);
BKEXPORT BkJSValue BKAPI BkObjectGetMember(BkJSObject o, const char *name);

/**
 * Writers
 */

BK_DECLARE_HANDLE(BkJSArrayWriter, JSArrayWriterImpl);
BK_DECLARE_HANDLE(BkJSObjectWriter, JSObjectWriterImpl);

/**
 * Context
 */

BKEXPORT BkJSContext BKAPI BkCreateJSContext(void);
BKEXPORT int BKAPI BkDestroyJSContext(BkJSContext context);

BKEXPORT BkJSObject BKAPI BkGetUserObject(BkJSContext context);

BKEXPORT int BKAPI BkEvaluate(BkJSContext context, const char *code, BkJSValue *retVal);

enum BkCallContext {
    BK_CTX_GLOBAL = 0,
    BK_CTX_USER_OBJECT
};

/**
 * Function Calling
 */

BK_DECLARE_HANDLE(BkJSCallerContext, JSCallerContextImpl);

BKEXPORT BkJSCallerContext BKAPI BkPrepareFunctionCall(BkJSContext context, int callContext, const char *functionName);
BKEXPORT int BKAPI BkPushInteger(BkJSCallerContext callerContext, int n);
BKEXPORT int BKAPI BkPushString(BkJSCallerContext callerContext, const char *s);
BKEXPORT int BKAPI BkPushStringPiece(BkJSCallerContext callerContext, const char *s, size_t l);
BKEXPORT int BKAPI BkCallFunction(BkJSCallerContext callerContext, BkJSValue *retVal);

/**
 * Function Extending
 */

BK_DECLARE_HANDLE(BkJSCalleeContext, JSCalleeContextImpl);

typedef void (BKAPI * BkFunctionImpl)(BkJSCalleeContext, void *);

BKEXPORT int BKAPI BkRegisterFunction(BkJSContext context, int memberContext, const char *functionName, BkFunctionImpl impl, void *userData);

BKEXPORT BkJSObject BKAPI BkGetThis(BkJSCalleeContext context);
BKEXPORT unsigned BKAPI BkGetArgCount(BkJSCalleeContext context);
BKEXPORT int BKAPI BkGetArgType(BkJSCalleeContext context, unsigned argIndex);
BKEXPORT int BKAPI BkGetArgAsBoolean(BkJSCalleeContext context, unsigned argIndex, bool_t *dst);
BKEXPORT int BKAPI BkGetArgAsInteger(BkJSCalleeContext context, unsigned argIndex, int *dst);
BKEXPORT int BKAPI BkGetArgAsNumber(BkJSCalleeContext context, unsigned argIndex, double *dst);
BKEXPORT int BKAPI BkGetArgAsString(BkJSCalleeContext context, unsigned argIndex, struct BkBuffer *dst);
BKEXPORT BkJSObject BKAPI BkGetArgAsObject(BkJSCalleeContext context, unsigned argIndex);
BKEXPORT int BKAPI BkReturnBoolean(BkJSCalleeContext context, bool_t retVal);
BKEXPORT int BKAPI BkReturnNumber(BkJSCalleeContext context, double retVal);
BKEXPORT int BKAPI BkReturnString(BkJSCalleeContext context, const char *retVal);
BKEXPORT int BKAPI BkReturnStringPiece(BkJSCalleeContext context, const char *retVal, size_t l);

enum BkConsoleMessageType {
    BK_CONSOLE_LOG = 0,
    BK_CONSOLE_WARN,
    BK_CONSOLE_ERROR
};

#ifdef __cplusplus
} // extern "C"
#endif

#endif // BLINKIT_SDK_JS_H
