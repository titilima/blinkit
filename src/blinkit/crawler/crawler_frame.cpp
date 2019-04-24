// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: crawler_frame.cpp
// Description: CrawlerFrame Class
//      Author: Ziming Li
//     Created: 2019-04-11
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "crawler_frame.h"

#include "app/app_impl.h"
#include "crawler/crawler_dom_window.h"

using namespace blink;

namespace BlinKit {

CrawlerFrame::CrawlerFrame(FrameLoaderClient *client) : LocalFrameImpl(client, nullptr, nullptr)
{
    // Nothing
}

PassRefPtrWillBeRawPtr<CrawlerFrame> CrawlerFrame::Create(FrameLoaderClient *client)
{
    RefPtrWillBeRawPtr<CrawlerFrame> frame = adoptRefWillBeNoop(new CrawlerFrame(client));
    return frame.release();
}

PassRefPtrWillBeRawPtr<LocalDOMWindowImpl> CrawlerFrame::CreateNewDOMWindow(void)
{
    RefPtrWillBeRawPtr<CrawlerDOMWindow> domWindow = adoptRefWillBeNoop(new CrawlerDOMWindow(*this));
    return domWindow.release();
}

} // namespace BlinKit

namespace blink {

Settings& Frame::CrawlerSettings(void)
{
    return BlinKit::AppImpl::Get().CrawlerSettings();
}

} // namespace blink
