// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: http_loader_task.h
// Description: HTTPLoaderTask Class
//      Author: Ziming Li
//     Created: 2019-03-21
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_HTTP_LOADER_TASK_H
#define BLINKIT_BLINKIT_HTTP_LOADER_TASK_H

#pragma once

#include <optional>
#include "bk_crawler.h"
#include "bk_http.h"
#include "bkcommon/controller_impl.h"
#include "blinkit/loader/loader_task.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_request.h"
#include "third_party/blink/renderer/platform/wtf/text/atomic_string.h"
#include "url/gurl.h"

namespace blink {
class ResourceResponse;
}

namespace BlinKit {

class HTTPLoaderTask final : public LoaderTask, public ControllerImpl
{
public:
    HTTPLoaderTask(BkCrawler crawler, const std::shared_ptr<base::SingleThreadTaskRunner> &taskRunner, blink::WebURLLoaderClient *client);
    ~HTTPLoaderTask(void) override;
private:
    BkRequest CreateRequest(const std::string &URL);
    AtomicString GetResponseHeader(const AtomicString &name) const;

    bool ProcessHijackRequest(const std::string &URL);
    bool ProcessHijackResponse(void);
    void ProcessRequestComplete(void);
    void PopulateHijackedResponse(const std::string &URL, const std::string &hijack);
    void PopulateResourceResponse(blink::ResourceResponse &response) const;
    void DoContinue(void);
    void DoCancel(void);

    void RequestComplete(BkResponse response);
    static void BKAPI RequestCompleteImpl(BkResponse response, void *userData);
    void RequestFailed(int errorCode);
    static void BKAPI RequestFailedImpl(int errorCode, void *userData);
    bool RequestRedirect(BkResponse response, BkRequest request);
    static bool_t BKAPI RequestRedirectImpl(BkResponse response, BkRequest request, void *userData);

    // LoaderTask
    int Run(const blink::ResourceRequest &request) override;
    // ControllerImpl
    int Release(void) override { return CancelWork(); }
    int ContinueWorking(void) override;
    int CancelWork(void) override;

    BkCrawler m_crawler;
    GURL m_url;
    blink::HijackType m_hijackType = blink::HijackType::kOther;
    ResponseImpl *m_response = nullptr;

    bool m_callingCrawler = false;
    std::optional<bool> m_cancel;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_HTTP_LOADER_TASK_H
