// -------------------------------------------------
// BlinKit - BkLogin Library
// -------------------------------------------------
//   File Name: http_request_task.h
// Description: HTTPRequestTask Class
//      Author: Ziming Li
//     Created: 2020-06-04
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BKLOGIN_HTTP_REQUEST_TASK_H
#define BLINKIT_BKLOGIN_HTTP_REQUEST_TASK_H

#pragma once

#include "blinkit/login/tasks/request_task_base.h"

namespace BlinKit {

class HTTPRequestTask final : public RequestTaskBase
{
public:
    HTTPRequestTask(SOCKET client, const char *leadChars, int leadSize);
private:
    std::string GetURL(void) const override;
    void AdjustHeaders(BkHTTPHeaderMap &headers, LoginProxyImpl &loginProxy) override;
};

} // namespace BlinKit

#endif // BLINKIT_BKLOGIN_HTTP_REQUEST_TASK_H
