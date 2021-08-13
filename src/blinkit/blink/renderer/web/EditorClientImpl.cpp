// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: EditorClientImpl.cpp
// Description: EditorClientImpl Class
//      Author: Ziming Li
//     Created: 2021-08-09
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2006, 2007 Apple, Inc.  All rights reserved.
 * Copyright (C) 2012 Google, Inc.  All rights reserved.
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

#include "web/EditorClientImpl.h"

#include "core/editing/SelectionType.h"
#if 0 // BKTODO:
#include "public/web/WebContentSettingsClient.h"
#include "public/web/WebFrameClient.h"
#include "public/web/WebViewClient.h"
#include "web/WebLocalFrameImpl.h"
#include "web/WebViewImpl.h"
#endif

namespace blink {

EditorClientImpl::EditorClientImpl(WebViewImpl* webview)
    : m_webView(webview)
{
}

EditorClientImpl::~EditorClientImpl()
{
}

void EditorClientImpl::respondToChangedSelection(LocalFrame* frame, SelectionType selectionType)
{
    ASSERT(false); // BKTODO:
#if 0
    WebLocalFrameImpl* webFrame = WebLocalFrameImpl::fromFrame(frame);
    if (webFrame->client())
        webFrame->client()->didChangeSelection(selectionType != RangeSelection);
#endif
}

void EditorClientImpl::respondToChangedContents()
{
    ASSERT(false); // BKTODO:
#if 0
    if (m_webView->client())
        m_webView->client()->didChangeContents();
#endif
}

bool EditorClientImpl::canCopyCut(LocalFrame* frame, bool defaultValue) const
{
    ASSERT(false); // BKTODO:
    return true;
#if 0
    WebLocalFrameImpl* webFrame = WebLocalFrameImpl::fromFrame(frame);
    if (!webFrame->contentSettingsClient())
        return defaultValue;
    return webFrame->contentSettingsClient()->allowWriteToClipboard(defaultValue);
#endif
}

bool EditorClientImpl::canPaste(LocalFrame* frame, bool defaultValue) const
{
    ASSERT(false); // BKTODO:
    return true;
#if 0
    WebLocalFrameImpl* webFrame = WebLocalFrameImpl::fromFrame(frame);
    if (!webFrame->contentSettingsClient())
        return defaultValue;
    return webFrame->contentSettingsClient()->allowReadFromClipboard(defaultValue);
#endif
}

bool EditorClientImpl::handleKeyboardEvent()
{
    ASSERT(false); // BKTODO: return m_webView->client() && m_webView->client()->handleCurrentKeyboardEvent();
    return false;
}

} // namespace blink
