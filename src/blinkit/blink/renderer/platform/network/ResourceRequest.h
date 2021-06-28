#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: ResourceRequest.h
// Description: ResourceRequest Class
//      Author: Ziming Li
//     Created: 2021-06-25
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_RESOURCE_REQUEST_H
#define BLINKIT_BLINKIT_RESOURCE_REQUEST_H

#include "third_party/zed/include/zed/net/url.hpp"

namespace blink {

class ResourceRequest final
{
public:
#ifdef BLINKIT_CRAWLER_ENABLED
    bool ForCrawler(void) const;
#endif

    const zed::url& url(void) const { return m_URL; }
private:
    zed::url m_URL;
};

} // namespace blink

#endif // BLINKIT_BLINKIT_RESOURCE_REQUEST_H
