// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: http_loader.h
// Description: HTTPLoader Class
//      Author: Ziming Li
//     Created: 2019-08-30
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_HTTP_LOADER_H
#define BLINKIT_BLINKIT_HTTP_LOADER_H

#pragma once

#include "sdk/include/BlinKit.h"
#include "response_task.h"

namespace BlinKit {

class CrawlerImpl;

class HTTPLoader final : public ResponseTask, public BkRequestClient, public BkCrawlerResponse
{
public:
    HTTPLoader(CrawlerImpl &crawler, blink::WebURLLoader *loader, blink::WebURLLoaderClient *client, ResponseData &responseData);

    void SetMethod(const std::string &method) { m_method = method; }
    void SetURL(const std::string &URL) { m_currentURL = URL; }
    void AddHeader(const std::string &name, const std::string &value) { m_headers[name] = value; }
    void SetTaskRunner(blink::WebTaskRunner *taskRunner) { m_taskRunner = taskRunner; }

    int Load(void);
private:
    void ApplyCookies(const BkResponse &response);

    // BkRequestClient
    void BKAPI RequestComplete(const BkResponse &response) override;
    void BKAPI RequestFailed(int errorCode) override;
    bool BKAPI RequestRedirect(const BkResponse &response) override;
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
    std::string m_method;
    std::string m_currentURL;
    std::unordered_map<std::string, std::string> m_headers;
    std::vector<std::string> m_cookies;
    blink::WebTaskRunner *m_taskRunner = nullptr;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_HTTP_LOADER_H
