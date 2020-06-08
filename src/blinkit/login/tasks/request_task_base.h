// -------------------------------------------------
// BlinKit - BkLogin Library
// -------------------------------------------------
//   File Name: request_task_base.h
// Description: RequestTaskBase Class
//      Author: Ziming Li
//     Created: 2020-06-04
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BKLOGIN_REQUEST_TASK_BASE_H
#define BLINKIT_BKLOGIN_REQUEST_TASK_BASE_H

#pragma once

#include <string_view>
#include "blinkit/common/bk_http_header_map.h"
#include "blinkit/login/socket_wrapper.h"
#include "blinkit/login/tasks/login_task.h"

namespace BlinKit {

class ResponseTaskBase;

class RequestTaskBase : public LoginTask
{
protected:
    RequestTaskBase(const std::shared_ptr<SocketWrapper> &socketWrapper, const std::string_view &leadChars = std::string_view());

    const std::string& RequestURI(void) const { return m_requestURI; }

    virtual void AdjustHeaders(BkHTTPHeaderMap &headers, LoginProxyImpl &loginProxy);

    std::shared_ptr<SocketWrapper> m_socketWrapper;
private:
    bool ParseRequest(void);
    bool ParseHeaders(const std::string_view &s);
    LoginTask* DoRealRequest(LoginProxyImpl &loginProxy);

    virtual std::string GetURL(void) const = 0;

    LoginTask* Execute(LoginProxyImpl &loginProxy) override final;

    std::string m_leadData;
    std::string m_requestMethod, m_requestURI;
    BkHTTPHeaderMap m_requestHeaders;
    std::string m_requestBody;
};

} // namespace BlinKit

#endif // BLINKIT_BKLOGIN_REQUEST_TASK_BASE_H
