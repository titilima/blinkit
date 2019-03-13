// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: view_impl.cpp
// Description: ViewImpl Class
//      Author: Ziming Li
//     Created: 2019-03-11
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "view_impl.h"

#include "platform/weborigin/KURL.h"

using namespace blink;

namespace BlinKit {

ViewImpl::ViewImpl(BkViewClient &client) : m_client(client)
{
    // Nothing
}

ViewImpl::~ViewImpl(void)
{
    // Nothing
}

int BKAPI ViewImpl::Load(const char *URI)
{
    KURL u(ParsedURLString, URI);
    if (u.protocolIsInHTTPFamily())
        return BkError::URIError;
    return BrowserImpl::Load(u);
}

} // namespace BlinKit
