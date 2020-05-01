// -------------------------------------------------
// BlinKit SDK
// -------------------------------------------------
//   File Name: bk_crawler.h
//      Author: Ziming Li
//     Created: 2019-09-04
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_SDK_CRAWLER_H
#define BLINKIT_SDK_CRAWLER_H

#pragma once

#include <stdint.h>
#include "bk_def.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Crawler
 */

BK_DECLARE_HANDLE(BkCrawler, CrawlerImpl);

enum BkCrawlerConfig {
    BK_CFG_OBJECT_SCRIPT = 0,

    // Return true:
    //   * Empty string: BK_PROXY_SYSTEM_DEFAULT.
    //   * Non-empty string: BK_PROXY_USER_SPECIFIED.
    // Return false: BK_PROXY_DIRECT.
    // Note:
    //   A null `GetConfig` callback will apply BK_PROXY_SYSTEM_DEFAULT.
    BK_CFG_REQUEST_PROXY,

    BK_CFG_REQUEST_COOKIE,
    BK_CFG_USER_AGENT,
    BK_CFG_SCRIPT_DISABLED,
    BK_CFG_LOGGED_IN_HTML
};

struct BkCrawlerClient {
    size_t SizeOfStruct; // sizeof(BkCrawlerClient)
    void *UserData;
    void (BKAPI * DocumentReady)(void *);
    bool_t (BKAPI * GetConfig)(int, struct BkBuffer *, void *);
    bool_t (BKAPI * HijackRequest)(const char *, struct BkBuffer *, void *);
    void (BKAPI * HijackResponse)(BkResponse, void *);
    void (BKAPI * RequestComplete)(BkResponse, BkWorkController, void *);
    void (BKAPI * DocumentReset)(void *);
    void (BKAPI * Error)(int, const char *, void *);
    void (BKAPI * ConsoleMessage)(int type, const char *, void *);
};

BKEXPORT BkCrawler BKAPI BkCreateCrawler(struct BkCrawlerClient *client);
BKEXPORT void BKAPI BkDestroyCrawler(BkCrawler crawler);

BKEXPORT int BKAPI BkRunCrawler(BkCrawler crawler, const char *URL);

BKEXPORT BkJSContext BKAPI BkGetScriptContextFromCrawler(BkCrawler crawler);

BKEXPORT void BKAPI BkHijackResponse(BkResponse response, const void *newBody, size_t length);

/**
 * Login Proxy
 */

struct BkLoginProxyClient {
    size_t SizeOfStruct; // sizeof(BkLoginProxyClient)
    void *UserData;
    void (BKAPI * SetCookie)(const char *, void *);
    bool_t (BKAPI * IsLoginSuccessful)(const char *, void *);
    bool_t (BKAPI * GetConfig)(int, struct BkBuffer *, void *);
};

BKEXPORT int BKAPI BkRunLoginProxy(const char *loginURL, uint16_t port, BkLoginProxyClient *client);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif // BLINKIT_SDK_CRAWLER_H
