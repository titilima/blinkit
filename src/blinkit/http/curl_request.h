// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: curl_request.h
// Description: CURLRequest Class
//      Author: Ziming Li
//     Created: 2019-09-10
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_CURL_REQUEST_H
#define BLINKIT_BLINKIT_CURL_REQUEST_H

#pragma once

#include <pthread.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include "request_impl.h"

namespace BlinKit {

class CURLRequest final : public RequestImpl
{
public:
    CURLRequest(const char *URL, const BkRequestClient &client);
    ~CURLRequest(void);
private:
    static CURLoption TranslateOption(const std::string &name);
    void* DoThreadWork(void);
    static void* ThreadProc(void *arg);
    static size_t WriteCallback(char *ptr, size_t, size_t nmemb, void *userData);

    // RequestImpl
    int Perform(void) override;
    ControllerImpl* GetController(void) override;
    void Cancel(void) override;

    CURL *m_curl;
    pthread_t m_thread = 0;
    curl_slist *m_headers = nullptr;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_CURL_REQUEST_H
