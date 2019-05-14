// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: HttpEquiv.cpp
// Description: HttpEquiv Class
//      Author: Ziming Li
//     Created: 2019-05-14
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/loader/HttpEquiv.h"

#include "core/dom/Document.h"
#include "core/dom/StyleEngine.h"
#include "core/frame/UseCounter.h"
#include "core/html/HTMLDocument.h"
#include "core/inspector/ConsoleMessage.h"
#include "core/loader/DocumentLoader.h"
#include "platform/network/HTTPParsers.h"
#include "platform/weborigin/KURL.h"

namespace blink {

void HttpEquiv::process(Document& document, const AtomicString& equiv, const AtomicString& content, bool inDocumentHeadElement)
{
    ASSERT(!equiv.isNull() && !content.isNull());

    if (equalIgnoringCase(equiv, "default-style")) {
        processHttpEquivDefaultStyle(document, content);
    } else if (equalIgnoringCase(equiv, "refresh")) {
        processHttpEquivRefresh(document, content);
    } else if (equalIgnoringCase(equiv, "set-cookie")) {
        processHttpEquivSetCookie(document, content);
    } else if (equalIgnoringCase(equiv, "content-language")) {
        document.setContentLanguage(content);
    } else if (equalIgnoringCase(equiv, "x-frame-options")) {
        processHttpEquivXFrameOptions(document, content);
    } else if (equalIgnoringCase(equiv, "accept-ch")) {
        processHttpEquivAcceptCH(document, content);
    } else if (equalIgnoringCase(equiv, "content-security-policy") || equalIgnoringCase(equiv, "content-security-policy-report-only")) {
        if (inDocumentHeadElement)
            processHttpEquivContentSecurityPolicy(document, equiv, content);
    }
}

void HttpEquiv::processHttpEquivContentSecurityPolicy(Document& document, const AtomicString& equiv, const AtomicString& content)
{
    // Nothing to do.
}

void HttpEquiv::processHttpEquivAcceptCH(Document& document, const AtomicString& content)
{
    if (!document.frame())
        return;

    UseCounter::count(document, UseCounter::ClientHintsMetaAcceptCH);
}

void HttpEquiv::processHttpEquivDefaultStyle(Document& document, const AtomicString& content)
{
    // The preferred style set has been overridden as per section
    // 14.3.2 of the HTML4.0 specification. We need to update the
    // sheet used variable and then update our style selector.
    // For more info, see the test at:
    // http://www.hixie.ch/tests/evil/css/import/main/preferred.html
    // -dwh
    document.styleEngine().setSelectedStylesheetSetName(content);
    document.styleEngine().setPreferredStylesheetSetName(content);
    document.styleResolverChanged();
}

void HttpEquiv::processHttpEquivRefresh(Document& document, const AtomicString& content)
{
    document.maybeHandleHttpRefresh(content, Document::HttpRefreshFromMetaTag);
}

void HttpEquiv::processHttpEquivSetCookie(Document& document, const AtomicString& content)
{
    // FIXME: make setCookie work on XML documents too; e.g. in case of <html:meta .....>
    if (!document.isHTMLDocument())
        return;

    // Exception (for sandboxed documents) ignored.
    toHTMLDocument(document).setCookie(content, IGNORE_EXCEPTION);
}

void HttpEquiv::processHttpEquivXFrameOptions(Document& document, const AtomicString& content)
{
    // Nothing to do.
}

}
