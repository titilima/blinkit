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
#include "blinkit/loader_tasks/loader_task.h"
#include "blinkit/misc/controller_impl.h"
#include "third_party/blink/renderer/platform/wtf/text/atomic_string.h"

namespace blink {
class ResourceResponse;
}

namespace BlinKit {

class HTTPLoaderTask final : public LoaderTask, public BkRequestClientImpl, public ControllerImpl
{
public:
    HTTPLoaderTask(BkCrawler crawler, const std::shared_ptr<base::SingleThreadTaskRunner> &taskRunner, blink::WebURLLoaderClient *client);
    ~HTTPLoaderTask(void) override;
private:
    AtomicString GetResponseHeader(const AtomicString &name) const;

    void ProcessRequestComplete(void);
    void PopulateResourceResponse(blink::ResourceResponse &response) const;
    void DoContinue(void);
    void DoCancel(void);

    // LoaderTask
    int Run(const blink::ResourceRequest &request) override;
    // BkRequestClientImpl
    void RequestComplete(BkResponse response) override;
    void RequestFailed(int errorCode) override;
    // ControllerImpl
    int Release(void) override { return CancelWork(); }
    int ContinueWorking(void) override;
    int CancelWork(void) override;

    BkCrawler m_crawler;
    std::shared_ptr<ResponseImpl> m_response;

    bool m_callingCrawler = false;
    std::optional<bool> m_cancel;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_HTTP_LOADER_TASK_H
