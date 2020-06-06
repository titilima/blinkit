// -------------------------------------------------
// BlinKit - BkLogin Library
// -------------------------------------------------
//   File Name: response_task_base.h
// Description: ResponseTaskBase Class
//      Author: Ziming Li
//     Created: 2020-06-05
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BKLOGIN_RESPONSE_TASK_BASE_H
#define BLINKIT_BKLOGIN_RESPONSE_TASK_BASE_H

#pragma once

#include "blinkit/login/tasks/login_task.h"

class ResponseImpl;

namespace BlinKit {

class BkHTTPHeaderMap;

class ResponseTaskBase : public LoginTask, public BkRequestClient
{
public:
    ~ResponseTaskBase(void) override;
protected:
    ResponseTaskBase(SOCKET s, LoginProxyImpl &loginProxy);

    virtual void AdjustHeaders(BkHTTPHeaderMap &headers, LoginProxyImpl &loginProxy);

    SOCKET m_socket;
private:
    bool ProcessLoginOK(LoginProxyImpl &loginProxy);
    void ProcessResponse(LoginProxyImpl &loginProxy);
    void ProcessRequestComplete(BkResponse response);
    void ProcessRequestFailed(int errorCode);

    static void BKAPI RequestCompleteImpl(BkResponse response, void *userData);
    static void BKAPI RequestFailedImpl(int errorCode, void *userData);

    virtual int Send(const char *buf, int bufSize) = 0;

    LoginTask* Execute(LoginProxyImpl &loginProxy) override;

    LoginProxyImpl &m_loginProxy;
    std::shared_ptr<ResponseImpl> m_response;
};

} // namespace BlinKit

#endif // BLINKIT_BKLOGIN_RESPONSE_TASK_BASE_H
