// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: apple_request.mm
// Description: AppleRequest Class
//      Author: Ziming Li
//     Created: 2019-08-13
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "apple_request.h"

namespace BlinKit {

AppleRequest::AppleRequest(const char *URL, BkRequestClient &client) : RequestImpl(URL, client)
{
    // Nothing
}

void AppleRequest::Cancel(void)
{
    assert(false); // BKTODO:
}

int BKAPI AppleRequest::Perform(void)
{
    assert(false); // BKTODO:
    return BkError::UnknownError;
}

BkRequestController* BKAPI AppleRequest::RequireLifecycleController(void)
{
    assert(false); // BKTODO:
    return nullptr;
}

void BKAPI AppleRequest::SetHeader(const char *name, const char *value)
{
    assert(false); // BKTODO:
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BkRequest* RequestImpl::CreateInstance(const char *URL, BkRequestClient &client)
{
    return new AppleRequest(URL, client);
}

} // namespace BlinKit
