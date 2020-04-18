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

#include <Foundation/Foundation.h>
#include "blinkit/http/request_impl.h"

namespace BlinKit {

class AppleRequest final : public RequestImpl
{
public:
    AppleRequest(const char *URL, const BkRequestClient &client);
    ~AppleRequest(void) override;
private:
    void RequestComplete(NSData *data, NSURLResponse *response, NSError *error);

    // RequestImpl
    int Perform(void) override;
    ControllerImpl* GetController(void) override;
    void Cancel(void) override;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_APPLE_REQUEST_H
