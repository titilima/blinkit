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
#if 0
class AppleRequest final : public RequestImpl
{
public:
    AppleRequest(const char *URL, const BkRequestClient &client);
    ~AppleRequest(void);
private:
    void RequestComplete(NSData *data, NSURLResponse *response, NSError *error);

    // BkRequest
    int Perform(void) override;
    // RequestImpl
    void Cancel(void) override;

    NSURLSessionTask *m_sessionTask;
};
#endif
} // namespace BlinKit

#endif // BLINKIT_BLINKIT_APPLE_REQUEST_H
