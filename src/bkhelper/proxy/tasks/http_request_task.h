// -------------------------------------------------
// BlinKit - BkHelper Library
// -------------------------------------------------
//   File Name: http_request_task.h
// Description: HTTPRequestTask Class
//      Author: Ziming Li
//     Created: 2020-06-04
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BKHELPER_HTTP_REQUEST_TASK_H
#define BLINKIT_BKHELPER_HTTP_REQUEST_TASK_H

#pragma once

#include "bkhelper/proxy/tasks/request_task_base.h"

namespace BlinKit {

class HTTPRequestTask final : public RequestTaskBase
{
public:
    HTTPRequestTask(SOCKET client, const char *leadChars, int leadSize);
private:
    std::string GetURL(void) const override;
    void AdjustHeaders(HttpHeaders &headers, ProxyImpl &proxy) override;
};

} // namespace BlinKit

#endif // BLINKIT_BKHELPER_HTTP_REQUEST_TASK_H
