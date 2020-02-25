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
};

struct BkCrawlerClient {
    void *UserData;
    void (BKAPI * GetConfig)(int, struct BkBuffer *, void *);
    void (BKAPI * RequestComplete)(BkResponse, BkWorkController, void *);
    void (BKAPI * DocumentReady)(void *);
    void (BKAPI * Error)(int, const char *, void *);
    void (BKAPI * ConsoleLog)(const char *, void *);
};

BKEXPORT BkCrawler BKAPI BkCreateCrawler(struct BkCrawlerClient *client);
BKEXPORT void BKAPI BkDestroyCrawler(BkCrawler crawler);

BKEXPORT int BKAPI BkRunCrawler(BkCrawler crawler, const char *URL);

BKEXPORT BkJSContext BKAPI BkGetScriptContextFromCrawler(BkCrawler crawler);

#ifdef __cplusplus
} // extern "C"

namespace BlinKit {

class BkCrawlerClientImpl : public BkClientImpl<BkCrawlerClientImpl, BkCrawlerClient>
{
    template <class T, typename C> friend class BkClientImpl;
protected:
    void Attach(BkCrawlerClient &rawClient) override
    {
        rawClient.GetConfig = GetConfigImpl;
        rawClient.RequestComplete = RequestCompleteImpl;
        rawClient.DocumentReady = DocumentReadyImpl;
        rawClient.Error = ErrorImpl;
    }
    virtual void RequestComplete(BkResponse response, BkWorkController controller)
    {
        BkControllerContinueWorking(controller);
    }
private:
    virtual void GetConfig(int cfg, std::string &dst)
    {
        assert(false); // Not implemented!
    }
    virtual void DocumentReady(void) = 0;
    virtual void Error(int errorCode, const char *URL)
    {
        assert(BK_ERR_SUCCESS == errorCode);
    }

    static void BKAPI GetConfigImpl(int cfg, BkBuffer *dst, void *userData)
    {
        std::string s;
        ToImpl(userData)->GetConfig(cfg, s);
        if (!s.empty())
            BkSetBufferData(dst, s.data(), s.length());
    }
    static void BKAPI RequestCompleteImpl(BkResponse response, BkWorkController controller, void *userData)
    {
        ToImpl(userData)->RequestComplete(response, controller);
    }
    static void BKAPI DocumentReadyImpl(void *userData)
    {
        ToImpl(userData)->DocumentReady();
    }
    static void BKAPI ErrorImpl(int errorCode, const char *URL, void *userData)
    {
        ToImpl(userData)->Error(errorCode, URL);
    }
};

} // namespace BlinKit
#endif // __cplusplus

#endif // BLINKIT_SDK_CRAWLER_H
