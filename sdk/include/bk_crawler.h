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

struct BkCrawlerClient {
    void *UserData;
    void (BKAPI * GetUserScript)(struct BkBuffer *, void *);
};

BKEXPORT BkCrawler BKAPI BkCreateCrawler(struct BkCrawlerClient *client);
BKEXPORT void BKAPI BkDestroyCrawler(BkCrawler crawler);

BKEXPORT int BKAPI BkRunCrawler(BkCrawler crawler, const char *URL);

#ifdef __cplusplus
} // extern "C"

namespace BlinKit {

class BkCrawlerClientImpl : public BkClientImpl<BkCrawlerClientImpl, BkCrawlerClient>
{
public:
    void Setup(BkCrawlerClient &rawClient)
    {
        rawClient.GetUserScript = GetUserScriptImpl;
    }
private:
    virtual std::string GetUserScript(void) = 0;

    static void BKAPI GetUserScriptImpl(BkBuffer *dst, void *userData)
    {
        std::string userScript = ToImpl(userData)->GetUserScript();
        BkSetBufferData(dst, userScript.data(), userScript.length());
    }
};

} // namespace BlinKit
#endif // __cplusplus

#endif // BLINKIT_SDK_CRAWLER_H
