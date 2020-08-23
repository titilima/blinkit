// -------------------------------------------------
// BlinKit - BkBase Library
// -------------------------------------------------
//   File Name: request_impl.h
// Description: RequestImpl Class
//      Author: Ziming Li
//     Created: 2018-09-27
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BKBASE_REQUEST_IMPL_H
#define BLINKIT_BKBASE_REQUEST_IMPL_H

#pragma once

#include <vector>
#include <curl/curl.h>
#include <curl/easy.h>
#include "bk_http.h"
#include "bkcommon/bk_http_header_map.h"

namespace BlinKit {
class HttpResponse;
}

class RequestImpl
{
public:
    static RequestImpl* CreateInstance(const char *URL, const BkRequestClient &client);
    virtual ~RequestImpl(void);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Exports
    int Perform(void);
    void SetMethod(const std::string &method) { m_method = method; }
    void SetHeader(const char *name, const char *value);
    void SetBody(const void *data, size_t dataLength);
    void SetTimeout(unsigned timeout) { m_timeoutInMs = timeout * 1000; }
    void SetProxy(int type, const char *proxy);

    class Controller;
    Controller* GetController(void) { return m_controller; }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
protected:
    RequestImpl(const char *URL, const BkRequestClient &client);

    unsigned long TimeoutInMs(void) const { return m_timeoutInMs; }
    int ProxyType(void) const { return m_proxyType; }
    const std::string& Proxy(void) const;

    void DoThreadWork(void);
private:
    virtual bool StartWorkThread(void) = 0;
    static CURLoption TranslateOption(const char *name);
    static size_t HeaderCallback(char *ptr, size_t, size_t nmemb, void *userData);
    static size_t WriteCallback(char *ptr, size_t, size_t nmemb, void *userData);

    const std::string m_URL;
    BkRequestClient m_client;
    Controller *m_controller;

    std::string m_method;
    std::unordered_map<CURLoption, std::string> m_standardHeaders;
    BlinKit::BkHTTPHeaderMap m_userHeaders;
    std::vector<unsigned char> m_body;

    CURL *m_curl = nullptr;;
    curl_slist *m_headersList = nullptr;
    BlinKit::HttpResponse *m_response = nullptr;

    unsigned long m_timeoutInMs;
    int m_proxyType = BK_PROXY_SYSTEM_DEFAULT;
    std::string m_proxy;
};

#endif // BLINKIT_BKBASE_REQUEST_IMPL_H
