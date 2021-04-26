// -------------------------------------------------
// BlinKit SDK
// -------------------------------------------------
//   File Name: bk_def.h
//      Author: Ziming Li
//     Created: 2019-09-04
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_SDK_DEF_H
#define BLINKIT_SDK_DEF_H

#pragma once

#include <stddef.h>
#include <string.h>

#ifdef _WIN32
#   define BKAPI    __stdcall
#   ifndef BLINKIT_EXPORTS
#       define BKEXPORT __declspec(dllimport)
#   endif
typedef wchar_t BkPathChar;
#   define BK_PATH_CHAR_DEFINED
#endif

#ifdef __linux__
#   ifdef BLINKIT_EXPORTS
#       define BKEXPORT __attribute__((visibility("default")))
#   endif
#endif

#ifndef BKAPI
#   define BKAPI
#endif
#ifndef BKEXPORT
#   define BKEXPORT
#endif

#ifndef BK_PATH_CHAR_DEFINED
typedef char    BkPathChar;
#endif

#ifdef __cplusplus
#   define BK_DECLARE_HANDLE(name, impl)    class impl; typedef impl *name
#else
#   define BK_DECLARE_HANDLE(name, impl)    struct impl; typedef struct impl *name
#endif

#ifdef __cplusplus
typedef bool    bool_t;
#elif !defined(BK_BOOLEAN_TYPE_DEFINED)
typedef unsigned char   bool_t;
enum BkBooleanValues {
    false = 0, true = 1
};
#   define BK_BOOLEAN_TYPE_DEFINED
#endif

enum BkError {
    BK_ERR_SUCCESS = 0,
    BK_ERR_UNKNOWN,
    BK_ERR_PARAM,
    BK_ERR_FORBIDDEN,
    BK_ERR_NOT_FOUND,
    BK_ERR_NETWORK,
    BK_ERR_CANCELLED,
    BK_ERR_EXCEPTION,
    BK_ERR_EVAL,
    BK_ERR_RANGE,
    BK_ERR_REFERENCE,
    BK_ERR_SYNTAX,
    BK_ERR_TYPE,
    BK_ERR_URI
};

BK_DECLARE_HANDLE(BkJSContext, ContextImpl);
BK_DECLARE_HANDLE(BkRequest, RequestImpl);
BK_DECLARE_HANDLE(BkResponse, ResponseImpl);
BK_DECLARE_HANDLE(BkWorkController, ControllerImpl);

#ifdef __cplusplus
extern "C" {
#endif

typedef void (BKAPI * BkJSCallback)(BkJSContext, void *);

struct BkBuffer {
    void* (BKAPI * Allocator)(size_t, void *);
    void *UserData;
};

BKEXPORT void BKAPI BkSetBufferData(struct BkBuffer *buffer, const void *data, size_t size);

#define BkSetBufferString(buffer, s)    BkSetBufferData(buffer, (s), strlen(s));

struct BkSimpleBuffer {
    struct BkBuffer Buffer;
    union {
        const char *String;
        void *Blob;
    };
    size_t Size;
};

BKEXPORT struct BkBuffer* BKAPI BkInitializeSimpleBuffer(struct BkSimpleBuffer *buffer);
BKEXPORT void BKAPI BkFinalizeSimpleBuffer(struct BkSimpleBuffer *buffer);

BKEXPORT int BKAPI BkControllerContinueWorking(BkWorkController controller);
BKEXPORT int BKAPI BkControllerCancelWork(BkWorkController controller);
BKEXPORT int BKAPI BkReleaseController(BkWorkController controller);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif // BLINKIT_SDK_DEF_H
