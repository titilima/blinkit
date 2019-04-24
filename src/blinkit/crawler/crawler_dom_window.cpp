// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: crawler_dom_window.cpp
// Description: CrawlerDOMWindow Class
//      Author: Ziming Li
//     Created: 2019-04-12
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "crawler_dom_window.h"

#include "core/events/DOMWindowEventQueue.h"
#include "crawler/crawler_document.h"
#include "crawler/crawler_frame.h"

using namespace blink;

namespace BlinKit {

CrawlerDOMWindow::CrawlerDOMWindow(CrawlerFrame &frame) : m_frame(frame)
{
    // Nothing
}

PassRefPtrWillBeRawPtr<DocumentImpl> CrawlerDOMWindow::CreateDocumentInternal(const String &mimeType, const DocumentInit &init, bool forceXHTML)
{
    return adoptRefWillBeNoop(new CrawlerDocument(init));
}

Frame* CrawlerDOMWindow::frame(void) const
{
    return &m_frame;
}

} // namespace BlinKit
