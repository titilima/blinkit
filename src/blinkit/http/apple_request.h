// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: apple_request.h
// Description: AppleRequest Class
//      Author: Ziming Li
//     Created: 2019-08-13
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_APPLE_REQUEST_H
#define BLINKIT_BLINKIT_APPLE_REQUEST_H

#pragma once

#include "request_impl.h"

namespace BlinKit {

class AppleRequest final : public RequestImpl
{
public:
    AppleRequest(const char *URL, BkRequestClient &client);
private:
    // BkRequest
    int BKAPI Perform(void) override;
    void BKAPI SetHeader(const char *name, const char *value) override;
    BkRequestController* BKAPI RequireLifecycleController(void) override;
    // RequestImpl
    void Cancel(void) override;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_APPLE_REQUEST_H
