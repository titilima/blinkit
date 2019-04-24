// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: crawler_frame.h
// Description: CrawlerFrame Class
//      Author: Ziming Li
//     Created: 2019-04-11
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_CRAWLER_FRAME_H
#define BLINKIT_BLINKIT_CRAWLER_FRAME_H

#pragma once

#include "core/frame/local_frame_impl.h"

namespace BlinKit {

class CrawlerFrame final : public blink::LocalFrameImpl
{
public:
    static PassRefPtrWillBeRawPtr<CrawlerFrame> Create(blink::FrameLoaderClient *client);
private:
    CrawlerFrame(blink::FrameLoaderClient *client);

    Type GetType(void) const override { return Type::Crawler; }
    PassRefPtrWillBeRawPtr<blink::LocalDOMWindowImpl> CreateNewDOMWindow(void) override;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_CRAWLER_FRAME_H
