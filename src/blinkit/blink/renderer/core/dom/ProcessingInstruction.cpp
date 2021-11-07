// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: ProcessingInstruction.cpp
// Description: ProcessingInstruction Class
//      Author: Ziming Li
//     Created: 2021-07-21
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2000 Peter Kelly (pmk@post.com)
 * Copyright (C) 2006, 2008, 2009 Apple Inc. All rights reserved.
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

#include "core/dom/ProcessingInstruction.h"

#include "core/css/CSSStyleSheet.h"
#include "core/css/MediaList.h"
#include "core/css/StyleSheetContents.h"
#include "core/dom/Document.h"
#include "core/dom/IncrementLoadEventDelayCount.h"
#include "core/dom/StyleEngine.h"
#include "core/fetch/CSSStyleSheetResource.h"
// BKTODO: #include "core/fetch/FetchInitiatorTypeNames.h"
#include "core/fetch/FetchRequest.h"
#include "core/fetch/ResourceFetcher.h"
#if 0 // BKTODO:
#include "core/fetch/XSLStyleSheetResource.h"
#include "core/xml/DocumentXSLT.h"
#include "core/xml/XSLStyleSheet.h"
#include "core/xml/parser/XMLDocumentParser.h" // for parseAttributes()
#endif

using namespace BlinKit;

namespace blink {

inline ProcessingInstruction::ProcessingInstruction(Document& document, const String& target, const String& data)
    : CharacterData(document, data, CreateOther)
    , m_target(target)
    , m_loading(false)
    , m_alternate(false)
    , m_createdByParser(false)
    , m_isCSS(false)
    , m_isXSL(false)
    , m_listenerForXSLT(nullptr)
{
}

GCRefPtr<ProcessingInstruction> ProcessingInstruction::create(Document& document, const String& target, const String& data)
{
    return GCWrapShared(new ProcessingInstruction(document, target, data));
}

ProcessingInstruction::~ProcessingInstruction()
{
#if !ENABLE(OILPAN)
    if (m_sheet)
        clearSheet();

    // FIXME: ProcessingInstruction should not be in document here.
    // However, if we add ASSERT(!inDocument()), fast/xsl/xslt-entity.xml
    // crashes. We need to investigate ProcessingInstruction lifetime.
    if (inDocument() && m_isCSS)
        document().styleEngine().removeStyleSheetCandidateNode(this);
    clearEventListenerForXSLT();
#endif
}

EventListener* ProcessingInstruction::eventListenerForXSLT()
{
    if (!m_listenerForXSLT)
        return 0;

    return m_listenerForXSLT->toEventListener();
}

void ProcessingInstruction::clearEventListenerForXSLT()
{
    if (m_listenerForXSLT) {
        m_listenerForXSLT->detach();
        m_listenerForXSLT.clear();
    }
}

String ProcessingInstruction::nodeName() const
{
    return m_target;
}

Node::NodeType ProcessingInstruction::nodeType() const
{
    return PROCESSING_INSTRUCTION_NODE;
}

GCRefPtr<Node> ProcessingInstruction::cloneNode(bool /*deep*/)
{
    // FIXME: Is it a problem that this does not copy m_localHref?
    // What about other data members?
    return create(document(), m_target, m_data);
}

void ProcessingInstruction::didAttributeChanged()
{
    if (m_sheet)
        clearSheet();

    String href;
    String charset;
    if (!checkStyleSheet(href, charset))
        return;
    process(href, charset);
}

bool ProcessingInstruction::checkStyleSheet(String& href, String& charset)
{
    if (m_target != "xml-stylesheet" || !document().frame() || parentNode() != document())
        return false;

    ASSERT(false); // BKTODO:
#if 0
    // see http://www.w3.org/TR/xml-stylesheet/
    // ### support stylesheet included in a fragment of this (or another) document
    // ### make sure this gets called when adding from javascript
    bool attrsOk;
    const HashMap<String, String> attrs = parseAttributes(m_data, attrsOk);
    if (!attrsOk)
        return false;
    HashMap<String, String>::const_iterator i = attrs.find("type");
    String type;
    if (i != attrs.end())
        type = i->value;

    m_isCSS = type.isEmpty() || type == "text/css";
    m_isXSL = (type == "text/xml" || type == "text/xsl" || type == "application/xml" || type == "application/xhtml+xml" || type == "application/rss+xml" || type == "application/atom+xml");
    if (!m_isCSS && !m_isXSL)
        return false;

    href = attrs.get("href");
    charset = attrs.get("charset");
    String alternate = attrs.get("alternate");
    m_alternate = alternate == "yes";
    m_title = attrs.get("title");
    m_media = attrs.get("media");
#endif

    return !m_alternate || !m_title.isEmpty();
}

void ProcessingInstruction::process(const String& href, const String& charset)
{
    if (href.length() > 1 && href[0] == '#') {
        m_localHref = href.substring(1);
        ASSERT(false); // BKTODO:
#if 0
        // We need to make a synthetic XSLStyleSheet that is embedded.
        // It needs to be able to kick off import/include loads that
        // can hang off some parent sheet.
        if (m_isXSL && RuntimeEnabledFeatures::xsltEnabled()) {
            KURL finalURL(ParsedURLString, m_localHref);
            m_sheet = XSLStyleSheet::createEmbedded(this, finalURL);
            m_loading = false;
        }
#endif
        return;
    }

    clearResource();

    String url = document().completeURL(href).string();

    ResourcePtr<StyleSheetResource> resource;
    ASSERT(false); // BKTODO:
#if 0
    FetchRequest request(ResourceRequest(document().completeURL(href)), FetchInitiatorTypeNames::processinginstruction);
    if (m_isXSL) {
        if (RuntimeEnabledFeatures::xsltEnabled())
            resource = XSLStyleSheetResource::fetch(request, document().fetcher());
    } else {
        request.setCharset(charset.isEmpty() ? document().characterSet() : charset);
        resource = CSSStyleSheetResource::fetch(request, document().fetcher());
    }

    if (resource) {
        m_loading = true;
        if (!m_isXSL)
            document().styleEngine().addPendingSheet();
        setResource(resource);
    }
#endif
}

bool ProcessingInstruction::isLoading() const
{
    if (m_loading)
        return true;
    if (!m_sheet)
        return false;
    return m_sheet->isLoading();
}

bool ProcessingInstruction::sheetLoaded()
{
    if (!isLoading()) {
        ASSERT(false); // BKTODO:
#if 0
        if (!DocumentXSLT::sheetLoaded(document(), this))
            document().styleEngine().removePendingSheet(this);
#endif
        return true;
    }
    return false;
}

void ProcessingInstruction::setCSSStyleSheet(const String& href, const KURL& baseURL, const String& charset, const CSSStyleSheetResource* sheet)
{
    if (!inDocument()) {
        ASSERT(!m_sheet);
        return;
    }

    ASSERT(m_isCSS);
    CSSParserContext parserContext(document(), baseURL, charset);

    GCRefPtr<StyleSheetContents> newSheet = StyleSheetContents::create(href, parserContext);

    GCRefPtr<CSSStyleSheet> cssSheet = CSSStyleSheet::create(newSheet.release(), this);
    cssSheet->setDisabled(m_alternate);
    cssSheet->setTitle(m_title);
    ASSERT(false); // BKTODO: cssSheet->setMediaQueries(MediaQuerySet::create(m_media));

    m_sheet = cssSheet.release();

    // We don't need the cross-origin security check here because we are
    // getting the sheet text in "strict" mode. This enforces a valid CSS MIME
    // type.
    parseStyleSheet(sheet->sheetText());
}

void ProcessingInstruction::setXSLStyleSheet(const String& href, const KURL& baseURL, const String& sheet)
{
    if (!inDocument()) {
        ASSERT(!m_sheet);
        return;
    }

    ASSERT(m_isXSL);
    ASSERT(false); // BKTODO:
#if 0
    m_sheet = XSLStyleSheet::create(this, href, baseURL);
    RefPtrWillBeRawPtr<Document> protect(&document());
    OwnPtr<IncrementLoadEventDelayCount> delay = IncrementLoadEventDelayCount::create(document());
    parseStyleSheet(sheet);
#endif
}

void ProcessingInstruction::parseStyleSheet(const String& sheet)
{
    ASSERT(false); // BKTODO:
#if 0
    if (m_isCSS)
        toCSSStyleSheet(m_sheet.get())->contents()->parseString(sheet);
    else if (m_isXSL)
        toXSLStyleSheet(m_sheet.get())->parseString(sheet);

    clearResource();
    m_loading = false;

    if (m_isCSS)
        toCSSStyleSheet(m_sheet.get())->contents()->checkLoaded();
    else if (m_isXSL)
        toXSLStyleSheet(m_sheet.get())->checkLoaded();
#endif
}

Node::InsertionNotificationRequest ProcessingInstruction::insertedInto(ContainerNode* insertionPoint)
{
    CharacterData::insertedInto(insertionPoint);
    if (!insertionPoint->inDocument())
        return InsertionDone;

    String href;
    String charset;
    bool isValid = checkStyleSheet(href, charset);
    ASSERT(false); // BKTODO:
#if 0
    if (!DocumentXSLT::processingInstructionInsertedIntoDocument(document(), this))
        document().styleEngine().addStyleSheetCandidateNode(this, m_createdByParser);
#endif
    if (isValid)
        process(href, charset);
    return InsertionDone;
}

void ProcessingInstruction::removedFrom(ContainerNode* insertionPoint)
{
    CharacterData::removedFrom(insertionPoint);
    if (!insertionPoint->inDocument())
        return;

    ASSERT(false); // BKTODO:
#if 0
    // No need to remove XSLStyleSheet from StyleEngine.
    if (!DocumentXSLT::processingInstructionRemovedFromDocument(document(), this))
        document().styleEngine().removeStyleSheetCandidateNode(this);
#endif

    GCRefPtr<StyleSheet> removedSheet = m_sheet;
    if (m_sheet) {
        ASSERT(m_sheet->ownerNode() == this);
        clearSheet();
    }

    // No need to remove pending sheets.
    clearResource();

    // If we're in document teardown, then we don't need to do any notification of our sheet's removal.
    if (document().isActive())
        document().removedStyleSheet(removedSheet.get());
}

void ProcessingInstruction::clearSheet()
{
    ASSERT(m_sheet);
    if (m_sheet->isLoading())
        document().styleEngine().removePendingSheet(this);
    GCRefPtr<StyleSheet> releasedSheet(m_sheet.release());
    releasedSheet->clearOwnerNode();
}

StyleSheet* ProcessingInstruction::sheet(void) const
{
    return m_sheet.get();
}

DEFINE_TRACE(ProcessingInstruction)
{
    visitor->trace(m_sheet);
    visitor->trace(m_listenerForXSLT);
    CharacterData::trace(visitor);
}

} // namespace
