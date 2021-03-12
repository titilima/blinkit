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
BKEXPORT void BKAPI BkReleaseCookieJar(BkCookieJar cookieJar);

BKEXPORT void BKAPI BkGetCookie(BkCookieJar cookieJar, const char *URL, struct BkBuffer *dst);
BKEXPORT bool BKAPI BkSetCookie(BkCookieJar cookieJar, const char *setCookieHeader, const char *URL);

BKEXPORT void BKAPI BkClearCookieJar(BkCookieJar cookieJar);

/**
 * Crawler
 */

BK_DECLARE_HANDLE(BkCrawler, CrawlerImpl);

enum BkCrawlerConfig {
    BK_CFG_OBJECT_SCRIPT = 0,
    BK_CFG_USER_AGENT,
};

enum BkScriptMode {
    BK_ALWAYS_ENABLE_SCRIPT = 0,
    BK_ALWAYS_DISABLE_SCRIPT,
    BK_ENABLE_SCRIPT_ONCE,
    BK_DISABLE_SCRIPT_ONCE
};

struct BkCrawlerClient {
    size_t SizeOfStruct; // sizeof(BkCrawlerClient)
    void *UserData;

    /**
     * Fires when a document is completely loaded and initialized.
     *   - REQUIRED.
     *   - Thread: client.
     */
    void (BKAPI * DocumentReady)(void *);

    /**
     * Fires after a main(HTML) request completed.
     *   - Thread: BlinKit.
     *   - Call BkHijackResponse to modify the HTML.
     *   - Call BkControllerContinueWorking to continue loading & initializing DOM.
     *   - Call BkControllerCancelWork to ignore the next loader steps.
     */
    void (BKAPI * RequestComplete)(BkResponse, BkWorkController, void *);

    /**
     * Get configurations for the crawler.
     *   - Thread: BlinKit.
     *   - See also: BkCrawlerConfig.
     */
    bool_t (BKAPI * GetConfig)(int, struct BkBuffer *, void *);

    /**
     * Get cookies for the URL being requested.
     *   - Thread: BlinKit.
     */
    bool_t (BKAPI * GetCookies)(const char *URL, const char *cookiesFromJar, struct BkBuffer *cookiesToSet, void *);

    /**
     * Get script mode for the URL being requested.
     *   - Thread: BlinKit.
     *   - See also: BkScriptMode.
     */
    int (BKAPI * GetScriptMode)(const char *URL, void *);

    /**
     * Hijack script requests before performing.
     *   - Thread: BlinKit.
     *   - Return true to use the hijacked data as the script, and the request will not be performed.
     */
    bool_t (BKAPI * HijackRequest)(const char *URL, struct BkBuffer *hijacked, void *);

    /**
     * Modify requests before performing, e.g. adding or replacing HTTP headers.
     *   - Thread: BlinKit.
     */
    void (BKAPI * ModifyRequest)(const char *URL, BkRequest, void *);

    /**
     * Hijack script responses.
     *   - Thread: BlinKit.
     *   - Call BkHijackResponse to modify the script content.
     */
    void (BKAPI * HijackResponse)(BkResponse, void *);

    /**
     * Fires when a new `window.document` object is initialized.
     *   - Thread: BlinKit.
     */
    void (BKAPI * DocumentReset)(BkJSContext, void *);

    /**
     * Fires when an error occurs.
     *   - REQUIRED.
     *   - Thread: client.
     *   - See also: BkError.
     */
    void (BKAPI * Error)(int code, const char *message, void *);

    /**
     * Fires when `window.console` outputs a string.
     *   - Thread: BlinKit.
     *   - See also: BkConsoleMessageType.
     */
    void (BKAPI * ConsoleMessage)(int type, const char *, void *);
};

BKEXPORT BkCrawler BKAPI BkCreateCrawler(struct BkCrawlerClient *client);
BKEXPORT void BKAPI BkDestroyCrawler(BkCrawler crawler);

BKEXPORT void BKAPI BkCrawlerEnableCookies(BkCrawler crawler, BkCookieJar *cookieJar);
BKEXPORT void BKAPI BkSetCookieJar(BkCrawler crawler, BkCookieJar cookieJar);

BKEXPORT int BKAPI BkRunCrawler(BkCrawler crawler, const char *URL);

BKEXPORT int BKAPI BkCrawlerCallJS(BkCrawler crawler, BkJSCallback callback, void *userData);

/**
 * Miscellaneous
 */

BKEXPORT void BKAPI BkHijackResponse(BkResponse response, const void *newBody, size_t length);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif // BLINKIT_SDK_CRAWLER_H
