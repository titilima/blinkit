// -------------------------------------------------
// BlinKit SDK
// -------------------------------------------------
//   File Name: bk_http.h
//      Author: Ziming Li
//     Created: 2019-09-04
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_SDK_HTTP_H
#define BLINKIT_SDK_HTTP_H

#pragma once

#include "bk_def.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Request
 */

struct BkRequestClient {
    size_t SizeOfStruct; // sizeof(BkRequestClient)
    void *UserData;
    void (BKAPI * RequestComplete)(BkResponse, void *);
    void (BKAPI * RequestFailed)(int errorCode, void *);
    bool_t (BKAPI * RequestRedirect)(BkResponse, void *);
};

BKEXPORT BkRequest BKAPI BkCreateRequest(const char *URL, struct BkRequestClient *client);

BKEXPORT int BKAPI BkPerformRequest(BkRequest request, BkWorkController *controller);

BKEXPORT void BKAPI BkSetRequestMethod(BkRequest request, const char *method);
BKEXPORT void BKAPI BkSetRequestHeader(BkRequest request, const char *name, const char *value);
BKEXPORT void BKAPI BkSetRequestBody(BkRequest request, const void *data, size_t dataLength);
BKEXPORT void BKAPI BkSetRequestTimeout(BkRequest request, unsigned timeout /* in seconds */);
BKEXPORT void BKAPI BkSetRequestProxy(BkRequest request, int type, const char *proxy);

/**
 * Response
 */

BKEXPORT int BKAPI BkGetResponseStatusCode(BkResponse response);

enum ResponseData {
    BK_RESPONSE_CURRENT_URL = 0,
    BK_RESPONSE_ORIGINAL_URL,
    BK_RESPONSE_BODY
};

BKEXPORT int BKAPI BkGetResponseData(BkResponse response, int data, struct BkBuffer *dst);

BKEXPORT int BKAPI BkGetResponseHeader(BkResponse response, const char *name, struct BkBuffer *dst);

typedef bool_t (BKAPI * BkHttpHeaderEnumerator)(const char *, const char *, void *);
BKEXPORT int BKAPI BkEnumerateResponseHeaders(BkResponse response, BkHttpHeaderEnumerator enumerator, void *userData);

BKEXPORT size_t BKAPI BkGetResponseCookiesCount(BkResponse response);
BKEXPORT int BKAPI BkGetResponseCookie(BkResponse response, size_t index, struct BkBuffer *dst);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // BLINKIT_SDK_HTTP_H
