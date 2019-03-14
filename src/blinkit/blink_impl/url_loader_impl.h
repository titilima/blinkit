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

#include "public/platform/WebURLLoader.h"

namespace blink {
class KURL;
}

namespace BlinKit {

class LoaderTask;

class URLLoaderImpl final : public blink::WebURLLoader
{
public:
    URLLoaderImpl(void);
private:
    // blink::WebURLLoader
    void loadSynchronously(const blink::WebURLRequest &, blink::WebURLResponse&, blink::WebURLError&, blink::WebData& data) override;
    void loadAsynchronously(const blink::WebURLRequest &request, blink::WebURLLoaderClient *client) override;
    void cancel(void) override;
    void setDefersLoading(bool) override;
    void setLoadingTaskRunner(blink::WebTaskRunner *taskRunner) override { m_taskRunner = taskRunner; }

    blink::WebTaskRunner *m_taskRunner;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_URL_LOADER_IMPL_H
