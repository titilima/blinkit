// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: url_loader_impl.h
// Description: URLLoaderImpl Class
//      Author: Ziming Li
//     Created: 2019-03-13
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_URL_LOADER_IMPL_H
#define BLINKIT_BLINKIT_URL_LOADER_IMPL_H

#pragma once

#include "third_party/blink/public/platform/web_url_loader.h"

namespace base {
class SingleThreadTaskRunner;
}

namespace BlinKit {

class URLLoaderImpl final : public blink::WebURLLoader
{
public:
    URLLoaderImpl(const std::shared_ptr<base::SingleThreadTaskRunner> &taskRunner);
    ~URLLoaderImpl(void) override;
private:
    // blink::WebURLLoader
    void LoadAsynchronously(const blink::ResourceRequest &request, blink::WebURLLoaderClient *client) override;

    std::shared_ptr<base::SingleThreadTaskRunner> m_taskRunner;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_URL_LOADER_IMPL_H
