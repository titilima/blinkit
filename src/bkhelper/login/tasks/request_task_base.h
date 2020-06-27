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

#ifndef BLINKIT_BKHELPER_REQUEST_TASK_BASE_H
#define BLINKIT_BKHELPER_REQUEST_TASK_BASE_H

#pragma once

#include <string_view>
#include <unordered_map>
#include <utility>
#include "bkhelper/login/socket_wrapper.h"
#include "bkhelper/login/tasks/login_task.h"

namespace BlinKit {

class ResponseTaskBase;

class RequestTaskBase : public LoginTask
{
protected:
    RequestTaskBase(const std::shared_ptr<SocketWrapper> &socketWrapper, const std::string_view &leadChars = std::string_view());

    const std::string& RequestURI(void) const { return m_requestURI; }

    typedef std::unordered_map<std::string, std::string> HttpHeaders;
    virtual void AdjustHeaders(HttpHeaders &headers, LoginProxyImpl &loginProxy);

    std::shared_ptr<SocketWrapper> m_socketWrapper;
private:
    bool ParseRequest(void);
    bool ParseHeaders(const std::string_view &s);
    LoginTask* DoRealRequest(LoginProxyImpl &loginProxy);

    virtual std::string GetURL(void) const = 0;

    LoginTask* Execute(LoginProxyImpl &loginProxy) override final;

    std::string m_leadData;
    std::string m_requestMethod, m_requestURI;
    HttpHeaders m_requestHeaders;
    std::string m_requestBody;
};

} // namespace BlinKit

#endif // BLINKIT_BKHELPER_REQUEST_TASK_BASE_H
