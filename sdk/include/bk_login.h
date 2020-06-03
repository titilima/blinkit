// -------------------------------------------------
// BlinKit SDK
// -------------------------------------------------
//   File Name: bk_login.h
//      Author: Ziming Li
//     Created: 2020-05-31
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_SDK_LOGIN_H
#define BLINKIT_SDK_LOGIN_H

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

/**
 * Login Proxy
 */

BK_DECLARE_HANDLE(BkLoginProxy, LoginProxyImpl);

struct BkLoginProxyClient {
    size_t SizeOfStruct; // sizeof(BkLoginProxyClient)
    void *UserData;
    void (BKAPI * SetCookie)(const char *, void *);
    bool_t(BKAPI * IsLoginSuccessful)(const char *, void *);
    bool_t(BKAPI * GetConfig)(int, struct BkBuffer *, void *);
};

BKEXPORT BkLoginProxy BKAPI BkCreateLoginProxy(struct BkLoginProxyClient *client);

BKEXPORT int BKAPI BkRunLoginProxy(BkLoginProxy loginProxy, const char *loginURL, uint16_t port);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif // BLINKIT_SDK_LOGIN_H
