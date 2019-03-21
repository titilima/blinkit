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

#include "request_impl.h"

namespace BlinKit {

class RequestControllerImpl final : public BkRequestController
{
public:
    RequestControllerImpl(RequestImpl &request) : m_request(request) {}
    ~RequestControllerImpl(void) {
        m_request.Release();
    }
private:
    // BkRequestController
    void BKAPI Cancel(void) override {
        m_request.Cancel();
        delete this;
    }
    void BKAPI Release(void) override {
        delete this;
    }

    RequestImpl &m_request;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_REQUEST_CONTROLLER_IMPL_H
