// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: crawler_dom_window.h
// Description: CrawlerDOMWindow Class
//      Author: Ziming Li
//     Created: 2019-04-12
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_CRAWLER_DOM_WINDOW_H
#define BLINKIT_BLINKIT_CRAWLER_DOM_WINDOW_H

#pragma once

#include "core/frame/local_dom_window_impl.h"

namespace BlinKit {

class CrawlerFrame;

class CrawlerDOMWindow final : public blink::LocalDOMWindowImpl
{
public:
    CrawlerDOMWindow(CrawlerFrame &frame);
private:
    Type GetType(void) const override { return Type::Crawler; }
    blink::Frame* frame(void) const override;
    PassRefPtrWillBeRawPtr<blink::DocumentImpl> CreateDocumentInternal(const String &mimeType, const blink::DocumentInit &init, bool forceXHTML) override;

    CrawlerFrame &m_frame;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_CRAWLER_DOM_WINDOW_H
