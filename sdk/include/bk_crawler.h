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

#include "bk_def.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Cookie Jar
 */

BK_DECLARE_HANDLE(BkCookieJar, CookieJarImpl);

BKEXPORT BkCookieJar BKAPI BkCreateCookieJar(void);
BKEXPORT void BKAPI BkDestroyCookieJar(BkCookieJar cookieJar);

BKEXPORT void BKAPI BkGetCookie(BkCookieJar cookieJar, const char *URL, struct BkBuffer *dst);
BKEXPORT bool BKAPI BkSetCookie(BkCookieJar cookieJar, const char *setCookieHeader, const char *URL);

BKEXPORT void BKAPI BkClearCookieJar(BkCookieJar cookieJar);

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

    BK_CFG_USER_AGENT,
    BK_CFG_SCRIPT_DISABLED,
};

enum BkCookieJarMode {
    BK_CJM_OWNED  =  0, // Need not to call BkDestroyCookieJar
    BK_CJM_SHARED
};

struct BkCrawlerClient {
    size_t SizeOfStruct; // sizeof(BkCrawlerClient)
    void *UserData;
    void (BKAPI * DocumentReady)(void *); // Required
    bool_t (BKAPI * GetConfig)(int, struct BkBuffer *, void *);
    bool_t (BKAPI * GetCookie)(const char *URL, const char *cookiesFromJar, struct BkBuffer *cookiesToSet, void *);
    bool_t (BKAPI * HijackRequest)(const char *, struct BkBuffer *, void *);
    void (BKAPI * HijackResponse)(BkResponse, void *);
    void (BKAPI * RequestComplete)(BkResponse, BkWorkController, void *);
    void (BKAPI * DocumentReset)(void *);
    void (BKAPI * Error)(int, const char *, void *); // Required
    void (BKAPI * ConsoleMessage)(int type, const char *, void *);
};

BKEXPORT BkCrawler BKAPI BkCreateCrawler(struct BkCrawlerClient *client);
BKEXPORT void BKAPI BkDestroyCrawler(BkCrawler crawler);

BKEXPORT void BKAPI BkSetCookieJar(BkCrawler crawler, BkCookieJar cookieJar, int mode);

BKEXPORT int BKAPI BkRunCrawler(BkCrawler crawler, const char *URL);

BKEXPORT BkJSContext BKAPI BkGetScriptContextFromCrawler(BkCrawler crawler);

/**
 * Miscellaneous
 */

BKEXPORT void BKAPI BkHijackResponse(BkResponse response, const void *newBody, size_t length);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif // BLINKIT_SDK_CRAWLER_H
