// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: DOMImplementation.cpp
// Description: DOMImplementation Class
//      Author: Ziming Li
//     Created: 2019-02-05
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2001 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2004, 2005, 2006, 2007, 2008 Apple Inc. All rights reserved.
 * Copyright (C) 2006 Samuel Weinig (sam@webkit.org)
 * Copyright (C) 2008, 2009 Torch Mobile Inc. All rights reserved. (http://www.torchmobile.com/)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "core/dom/DOMImplementation.h"

#include "bindings/core/v8/ExceptionState.h"
#include "core/HTMLNames.h"
#include "core/SVGNames.h"
#include "core/css/CSSStyleSheet.h"
#include "core/css/MediaList.h"
#include "core/css/StyleSheetContents.h"
#include "core/dom/ContextFeatures.h"
#include "core/dom/DocumentInit.h"
#include "core/dom/DocumentType.h"
#include "core/dom/Element.h"
#include "core/dom/ExceptionCode.h"
#include "core/dom/Text.h"
#include "core/dom/XMLDocument.h"
#include "core/dom/custom/CustomElementRegistrationContext.h"
#include "core/frame/LocalFrame.h"
#include "core/frame/UseCounter.h"
#include "core/html/HTMLDocument.h"
#include "core/html/HTMLHeadElement.h"
#include "core/html/HTMLMediaElement.h"
#include "core/html/HTMLTitleElement.h"
#include "core/loader/FrameLoader.h"
#include "core/page/Page.h"
#include "platform/ContentType.h"
#include "platform/MIMETypeRegistry.h"
#include "platform/graphics/Image.h"
#include "platform/weborigin/SecurityOrigin.h"
#include "wtf/StdLibExtras.h"

namespace blink {

DOMImplementation::DOMImplementation(Document& document)
    : m_document(document)
{
}

PassRefPtrWillBeRawPtr<DocumentType> DOMImplementation::createDocumentType(const AtomicString& qualifiedName,
    const String& publicId, const String& systemId, ExceptionState& exceptionState)
{
    AtomicString prefix, localName;
    if (!Document::parseQualifiedName(qualifiedName, prefix, localName, exceptionState))
        return nullptr;

    return DocumentType::create(m_document, qualifiedName, publicId, systemId);
}

PassRefPtrWillBeRawPtr<XMLDocument> DOMImplementation::createDocument(const AtomicString& namespaceURI,
    const AtomicString& qualifiedName, DocumentType* doctype, ExceptionState& exceptionState)
{
    RefPtrWillBeRawPtr<XMLDocument> doc = nullptr;
    DocumentInit init = DocumentInit::fromContext(document().contextDocument());
    if (namespaceURI == SVGNames::svgNamespaceURI) {
        doc = XMLDocument::createSVG(init);
    } else if (namespaceURI == HTMLNames::xhtmlNamespaceURI) {
        doc = XMLDocument::createXHTML(init.withRegistrationContext(document().registrationContext()));
    } else {
        doc = XMLDocument::create(init);
    }

    doc->setSecurityOrigin(document().securityOrigin()->isolatedCopy());
    doc->setContextFeatures(document().contextFeatures());

    RefPtrWillBeRawPtr<Node> documentElement = nullptr;
    if (!qualifiedName.isEmpty()) {
        documentElement = doc->createElementNS(namespaceURI, qualifiedName, exceptionState);
        if (exceptionState.hadException())
            return nullptr;
    }

    if (doctype)
        doc->appendChild(doctype);
    if (documentElement)
        doc->appendChild(documentElement.release());

    return doc.release();
}

bool DOMImplementation::isXMLMIMEType(const String& mimeType)
{
    if (equalIgnoringCase(mimeType, "text/xml")
        || equalIgnoringCase(mimeType, "application/xml")
        || equalIgnoringCase(mimeType, "text/xsl"))
        return true;

    // Per RFCs 3023 and 2045, an XML MIME type is of the form:
    // ^[0-9a-zA-Z_\\-+~!$\\^{}|.%'`#&*]+/[0-9a-zA-Z_\\-+~!$\\^{}|.%'`#&*]+\+xml$

    int length = mimeType.length();
    if (length < 7)
        return false;

    if (mimeType[0] == '/' || mimeType[length - 5] == '/' || !mimeType.endsWith("+xml", TextCaseInsensitive))
        return false;

    bool hasSlash = false;
    for (int i = 0; i < length - 4; ++i) {
        UChar ch = mimeType[i];
        if (ch >= '0' && ch <= '9')
            continue;
        if (ch >= 'a' && ch <= 'z')
            continue;
        if (ch >= 'A' && ch <= 'Z')
            continue;
        switch (ch) {
        case '_':
        case '-':
        case '+':
        case '~':
        case '!':
        case '$':
        case '^':
        case '{':
        case '}':
        case '|':
        case '.':
        case '%':
        case '\'':
        case '`':
        case '#':
        case '&':
        case '*':
            continue;
        case '/':
            if (hasSlash)
                return false;
            hasSlash = true;
            continue;
        default:
            return false;
        }
    }

    return true;
}

bool DOMImplementation::isJSONMIMEType(const String& mimeType)
{
    if (mimeType.startsWith("application/json", TextCaseInsensitive))
        return true;
    if (mimeType.startsWith("application/", TextCaseInsensitive)) {
        size_t subtype = mimeType.find("+json", 12, TextCaseInsensitive);
        if (subtype != kNotFound) {
            // Just check that a parameter wasn't matched.
            size_t parameterMarker = mimeType.find(";");
            if (parameterMarker == kNotFound) {
                unsigned endSubtype = static_cast<unsigned>(subtype) + 5;
                return endSubtype == mimeType.length() || isASCIISpace(mimeType[endSubtype]);
            }
            return parameterMarker > subtype;
        }
    }
    return false;
}

static bool isTextPlainType(const String& mimeType)
{
    return mimeType.startsWith("text/", TextCaseInsensitive)
        && !(equalIgnoringCase(mimeType, "text/html")
            || equalIgnoringCase(mimeType, "text/xml")
            || equalIgnoringCase(mimeType, "text/xsl"));
}

bool DOMImplementation::isTextMIMEType(const String& mimeType)
{
    return MIMETypeRegistry::isSupportedJavaScriptMIMEType(mimeType) || isJSONMIMEType(mimeType) || isTextPlainType(mimeType);
}

PassRefPtrWillBeRawPtr<HTMLDocument> DOMImplementation::createHTMLDocument(const String& title)
{
    DocumentInit init = DocumentInit::fromContext(document().contextDocument())
        .withRegistrationContext(document().registrationContext());
    RefPtrWillBeRawPtr<HTMLDocument> d = HTMLDocument::create(init);
    d->open();
    d->write("<!doctype html><html><head></head><body></body></html>");
    if (!title.isNull()) {
        HTMLHeadElement* headElement = d->head();
        ASSERT(headElement);
        RefPtrWillBeRawPtr<HTMLTitleElement> titleElement = HTMLTitleElement::create(*d);
        headElement->appendChild(titleElement);
        titleElement->appendChild(d->createTextNode(title), ASSERT_NO_EXCEPTION);
    }
    d->setSecurityOrigin(document().securityOrigin()->isolatedCopy());
    d->setContextFeatures(document().contextFeatures());
    return d.release();
}

PassRefPtrWillBeRawPtr<Document> DOMImplementation::createDocument(const String& type, const DocumentInit& init, bool inViewSourceMode)
{
    ASSERT(!inViewSourceMode);
    if (type == "text/html")
        return HTMLDocument::create(init);
    if (type == "application/xhtml+xml")
        return XMLDocument::createXHTML(init);
    return HTMLDocument::create(init);
}

DEFINE_TRACE(DOMImplementation)
{
    visitor->trace(m_document);
}

}
