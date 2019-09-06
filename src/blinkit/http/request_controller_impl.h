// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: request_controller_impl.h
// Description: RequestControllerImpl Class
//      Author: Ziming Li
//     Created: 2019-03-21
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_REQUEST_CONTROLLER_IMPL_H
#define BLINKIT_BLINKIT_REQUEST_CONTROLLER_IMPL_H

#pragma once

#include "http/request_impl.h"
#include "misc/controller_impl.h"

namespace BlinKit {

class RequestControllerImpl final : public ControllerImpl
{
public:
    RequestControllerImpl(RequestImpl &request) : m_request(request) {}
    ~RequestControllerImpl(void)
    {
        m_request.Release();
    }
private:
    // ControllerImpl
    int Release(void) override
    {
        delete this;
        return BK_ERR_SUCCESS;
    }
    int ContinueWorking(void) override { return BK_ERR_FORBIDDEN; }
    int CancelWork(void) override
    {
        m_request.Cancel();
        delete this;
        return BK_ERR_SUCCESS;
    }

    RequestImpl &m_request;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_REQUEST_CONTROLLER_IMPL_H
