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

BK_DECLARE_HANDLE(BkCrawler, CrawlerImpl);

enum BkCrawlerConfig {
    BK_CFG_OBJECT_SCRIPT = 0,
    BK_CFG_USER_AGENT,
    BK_CFG_SCRIPT_DISABLED
};

struct BkCrawlerClient {
    void *UserData;
    void (BKAPI * GetConfig)(int, struct BkBuffer *, void *);
    bool_t (BKAPI * HijackRequest)(const char *, struct BkBuffer *, void *);
    void (BKAPI * HijackResponse)(BkResponse, void *);
    void (BKAPI * RequestComplete)(BkResponse, BkWorkController, void *);
    void (BKAPI * DocumentReset)(void *);
    void (BKAPI * DocumentReady)(void *);
    void (BKAPI * Error)(int, const char *, void *);
    void (BKAPI * ConsoleMessage)(int type, const char *, void *);
};

BKEXPORT BkCrawler BKAPI BkCreateCrawler(struct BkCrawlerClient *client);
BKEXPORT void BKAPI BkDestroyCrawler(BkCrawler crawler);

BKEXPORT int BKAPI BkRunCrawler(BkCrawler crawler, const char *URL);

BKEXPORT BkJSContext BKAPI BkGetScriptContextFromCrawler(BkCrawler crawler);

BKEXPORT void BKAPI BkHijackResponse(BkResponse response, const void *newBody, size_t length);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif // BLINKIT_SDK_CRAWLER_H
