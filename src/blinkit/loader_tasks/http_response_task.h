// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: http_response_task.h
// Description: HTTPResponseTask Class
//      Author: Ziming Li
//     Created: 2019-03-22
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_HTTP_RESPONSE_TASK_H
#define BLINKIT_BLINKIT_HTTP_RESPONSE_TASK_H

#pragma once

#include "sdk/include/BlinKit.h"
#include "response_task.h"

namespace BlinKit {

class CrawlerImpl;

class HTTPResponseTask final : public ResponseTask, public BkRequestClient, public BkCrawlerResponse
{
public:
    HTTPResponseTask(CrawlerImpl &crawler, blink::WebURLLoader *loader, blink::WebURLLoaderClient *client, ResponseData &responseData);

    void Setup(blink::WebTaskRunner *taskRunner) {
        m_taskRunner = taskRunner;
    }
private:
    // BkRequestClient
    void BKAPI RequestComplete(const BkResponse &response) override;
    void BKAPI RequestFailed(int errorCode) override;
    // BkResponse
    int BKAPI GetCurrentURL(BkBuffer &URL) const override;
    int BKAPI StatusCode(void) const override;
    int BKAPI GetHeader(const char *name, BkBuffer &value) const override;
    unsigned BKAPI CookiesCount(void) const override;
    int BKAPI GetCookie(unsigned i, BkBuffer &cookie) const override;
    int BKAPI GetBody(BkBuffer &body) const override;
    // BkCrawlerResponse
    void BKAPI SetBody(const char *body, size_t length) override;
    void BKAPI Continue(void) override;
    void BKAPI Cancel(void) override;

    CrawlerImpl &m_crawler;
    std::string m_currentURL;
    blink::WebTaskRunner *m_taskRunner = nullptr;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_HTTP_RESPONSE_TASK_H
