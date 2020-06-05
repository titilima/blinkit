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
#include "blinkit/login/tasks/login_task.h"

namespace BlinKit {

class ResponseTaskBase;

class RequestTaskBase : public LoginTask
{
public:
    ~RequestTaskBase(void) override;
protected:
    RequestTaskBase(SOCKET s, const std::string_view &leadChars = std::string_view());

    SOCKET DetachSocket(void);
    const std::string& RequestURI(void) const { return m_requestURI; }

    virtual void AdjustHeaders(BkHTTPHeaderMap &headers, LoginProxyImpl &loginProxy);

    SOCKET m_socket;
private:
    void ParseRequest(void);
    bool ParseHeaders(const std::string_view &s);
    LoginTask* DoRealRequest(LoginProxyImpl &loginProxy);

    virtual int Recv(char *buf, int bufSize) const = 0;
    virtual std::string GetURL(void) const = 0;
    virtual ResponseTaskBase* CreateResponseTask(LoginProxyImpl &loginProxy) = 0;

    LoginTask* Execute(LoginProxyImpl &loginProxy) override;

    std::string m_leadData;
    std::string m_requestMethod, m_requestURI;
    BkHTTPHeaderMap m_requestHeaders;
    std::string m_requestBody;
};

} // namespace BlinKit

#endif // BLINKIT_BKLOGIN_REQUEST_TASK_BASE_H
