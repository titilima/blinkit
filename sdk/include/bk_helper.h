// -------------------------------------------------
// BlinKit SDK
// -------------------------------------------------
//   File Name: bk_helper.h
//      Author: Ziming Li
//     Created: 2020-05-31
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_SDK_HELPER_H
#define BLINKIT_SDK_HELPER_H

#pragma once

#include <stdint.h>
#include "bk_def.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Private Key & Certificate
 */

BK_DECLARE_HANDLE(BkPrivateKey, PrivateKeyImpl);

BKEXPORT BkPrivateKey BKAPI BkCreatePrivateKey(void);
BKEXPORT void BKAPI BkDestroyPrivateKey(BkPrivateKey key);

BKEXPORT int BKAPI BkSavePrivateKey(BkPrivateKey key, const BkPathChar *fileName);
BKEXPORT int BKAPI BkSignPrivateKey(BkPrivateKey key, const char *commonName, int days, const BkPathChar *certFileName);

/**
 * Proxy
 */

BK_DECLARE_HANDLE(BkProxyRequest, ProxyRequestImpl);

enum BkProxyRequestData {
    BK_PROXY_REQUEST_URL = 0,
};

BKEXPORT int BKAPI BkGetProxyRequestData(BkProxyRequest request, int data, struct BkBuffer *dst);

BKEXPORT int BKAPI BkGetProxyRequestHeader(BkProxyRequest request, const char *name, struct BkBuffer *dst);
BKEXPORT void BKAPI BkSetProxyRequestHeader(BkProxyRequest request, const char *name, const char *value);

BK_DECLARE_HANDLE(BkProxy, ProxyImpl);

enum BkProxyConfig {
    BK_PROXY_CFG_EXIT_BODY = 0
};

struct BkProxyClient {
    size_t SizeOfStruct; // sizeof(BkProxyClient)
    void *UserData;
    void (BKAPI * GetConfig)(int, struct BkBuffer *, void *);
    void (BKAPI * BeforeRequestPerform)(BkProxyRequest, void *);
    void (BKAPI * RequestComplete)(BkResponse, void *);
};

BKEXPORT BkProxy BKAPI BkCreateProxy(struct BkProxyClient *client);
BKEXPORT int BKAPI BkSetupProxyCA(BkProxy proxy, const BkPathChar *privateKeyFile, const BkPathChar *certFile);
BKEXPORT int BKAPI BkRunProxyLoop(BkProxy proxy, uint16_t port);
BKEXPORT void BKAPI BkExitProxyLoop(BkProxy proxy);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif // BLINKIT_SDK_HELPER_H
