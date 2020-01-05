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
    void (BKAPI * RequestComplete)(BkResponse, BkWorkController, void *);
    void (BKAPI * DocumentReady)(void *);
    void (BKAPI * Error)(int, const char *, void *);
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
        rawClient.RequestComplete = RequestCompleteImpl;
        rawClient.DocumentReady = DocumentReadyImpl;
        rawClient.Error = ErrorImpl;
    }
protected:
    virtual void RequestComplete(BkResponse responce, BkWorkController controller)
    {
        BkControllerContinueWorking(controller);
    }
private:
    virtual std::string GetUserScript(void) = 0;
    virtual void DocumentReady(void) = 0;
    virtual void Error(int errorCode, const char *URL)
    {
        assert(BK_ERR_SUCCESS == errorCode);
    }

    static void BKAPI GetUserScriptImpl(BkBuffer *dst, void *userData)
    {
        std::string userScript = ToImpl(userData)->GetUserScript();
        BkSetBufferData(dst, userScript.data(), userScript.length());
    }
    static void BKAPI RequestCompleteImpl(BkResponse responce, BkWorkController controller, void *userData)
    {
        ToImpl(userData)->RequestComplete(responce, controller);
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
