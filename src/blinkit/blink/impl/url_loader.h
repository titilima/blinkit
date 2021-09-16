// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: url_loader.h
// Description: URLLoader Class
//      Author: Ziming Li
//     Created: 2019-03-13
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_URL_LOADER_H
#define BLINKIT_BLINKIT_URL_LOADER_H

#pragma once

#include "blinkit/blink/public/platform/WebURLLoader.h"

namespace BlinKit {

class URLLoader final : public blink::WebURLLoader
{
public:
    URLLoader(void) = default;
private:
    // blink::WebURLLoader
    void loadAsynchronously(const blink::ResourceRequest &request, blink::WebURLLoaderClient *client) override;
    void cancel(void) override;
    void setLoadingTaskRunner(const std::shared_ptr<blink::WebTaskRunner> &taskRunner) override { m_taskRunner = taskRunner; }

    std::shared_ptr<blink::WebTaskRunner> m_taskRunner;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_URL_LOADER_H
