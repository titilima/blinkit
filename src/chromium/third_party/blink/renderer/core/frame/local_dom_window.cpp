// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: local_dom_window.cpp
// Description: LocalDOMWindow Class
//      Author: Ziming Li
//     Created: 2019-09-16
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2006, 2007, 2008, 2010 Apple Inc. All rights reserved.
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies)
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "local_dom_window.h"

#include "blinkit/crawler/crawler_document.h"
#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/dom/document_init.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/frame/local_frame_client.h"

using namespace BlinKit;

namespace blink {

LocalDOMWindow::LocalDOMWindow(LocalFrame &frame) : DOMWindow(frame)
{
}

LocalDOMWindow::~LocalDOMWindow(void) = default;

void LocalDOMWindow::ClearDocument(void)
{
    if (!m_document)
        return;

    assert(false); // BKTODO:
}

LocalFrame* LocalDOMWindow::GetFrame(void) const
{
    return ToLocalFrame(DOMWindow::GetFrame());
}

Document* LocalDOMWindow::InstallNewDocument(const DocumentInit &init)
{
    LocalFrame *frame = GetFrame();
    assert(init.GetFrame() == frame);

    ClearDocument();

#ifdef BLINKIT_CRAWLER_ONLY
    assert(init.GetFrame()->Client()->IsCrawler());
    m_document = std::make_unique<CrawlerDocument>(init);
#else
    if (init.GetFrame()->Client()->IsCrawler())
        m_document = std::make_unique<CrawlerDocument>(init);
    else
        assert(false); // BKTODO:
#endif
    m_document->Initialize();

    if (nullptr != frame)
    {
        BKLOG("// BKTODO: frame->GetScriptController().UpdateDocument();");
#ifndef BLINKIT_CRAWLER_ONLY
        m_document->GetViewportData().UpdateViewportDescription();
#endif
    }

    return m_document.get();
}

void LocalDOMWindow::Reset(void)
{
    assert(false); // BKTODO:
}

}  // namespace blink
