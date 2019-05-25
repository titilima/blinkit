// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: ProcessingInstruction.cpp
// Description: ProcessingInstruction Class
//      Author: Ziming Li
//     Created: 2019-02-07
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
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
#include "core/fetch/FetchInitiatorTypeNames.h"
#include "core/fetch/FetchRequest.h"
#include "core/fetch/ResourceFetcher.h"
#include "core/fetch/XSLStyleSheetResource.h"

namespace blink {

inline ProcessingInstruction::ProcessingInstruction(Document& document, const String& target, const String& data)
    : CharacterData(document, data, CreateOther)
    , m_target(target)
    , m_loading(false)
    , m_alternate(false)
    , m_createdByParser(false)
    , m_isCSS(false)
    , m_listenerForXSLT(nullptr)
{
}

PassRefPtrWillBeRawPtr<ProcessingInstruction> ProcessingInstruction::create(Document& document, const String& target, const String& data)
{
    return adoptRefWillBeNoop(new ProcessingInstruction(document, target, data));
}

ProcessingInstruction::~ProcessingInstruction()
{
#if !ENABLE(OILPAN)
    if (m_sheet)
        clearSheet();

#ifdef BLINKIT_CRAWLER_ONLY
    assert(false); // BKTODO: Not reached!
#else
    // FIXME: ProcessingInstruction should not be in document here.
    // However, if we add ASSERT(!inDocument()), fast/xsl/xslt-entity.xml
    // crashes. We need to investigate ProcessingInstruction lifetime.
    if (inDocument() && m_isCSS)
        document().styleEngine().removeStyleSheetCandidateNode(this);
#endif
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

PassRefPtrWillBeRawPtr<Node> ProcessingInstruction::cloneNode(bool /*deep*/)
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
    ASSERT(false); // BKTODO: Not reached!
    return false;
#if 0
    if (m_target != "xml-stylesheet" || !document().frame() || parentNode() != document())
        return false;

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
    if (!m_isCSS)
        return false;

    href = attrs.get("href");
    charset = attrs.get("charset");
    String alternate = attrs.get("alternate");
    m_alternate = alternate == "yes";
    m_title = attrs.get("title");
    m_media = attrs.get("media");

    return !m_alternate || !m_title.isEmpty();
#endif
}

void ProcessingInstruction::process(const String& href, const String& charset)
{
#ifdef BLINKIT_CRAWLER_ONLY
    assert(false); // BKTODO: Not reached!
#else
    if (href.length() > 1 && href[0] == '#') {
        m_localHref = href.substring(1);
        return;
    }

    clearResource();

    String url = document().completeURL(href).string();

    ResourcePtr<StyleSheetResource> resource;
    FetchRequest request(ResourceRequest(document().completeURL(href)), FetchInitiatorTypeNames::processinginstruction);
    request.setCharset(charset.isEmpty() ? document().characterSet() : charset);
    resource = CSSStyleSheetResource::fetch(request, document().fetcher());

    if (resource) {
        m_loading = true;
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
    return !isLoading();
}

void ProcessingInstruction::setCSSStyleSheet(const String& href, const KURL& baseURL, const String& charset, const CSSStyleSheetResource* sheet)
{
#ifdef BLINKIT_CRAWLER_ONLY
    assert(false); // BKTODO: Not reached!
#else
    if (!inDocument()) {
        ASSERT(!m_sheet);
        return;
    }

    ASSERT(m_isCSS);
    CSSParserContext parserContext(document(), 0, baseURL, charset);

    RefPtrWillBeRawPtr<StyleSheetContents> newSheet = StyleSheetContents::create(href, parserContext);

    RefPtrWillBeRawPtr<CSSStyleSheet> cssSheet = CSSStyleSheet::create(newSheet, this);
    cssSheet->setDisabled(m_alternate);
    cssSheet->setTitle(m_title);
    cssSheet->setMediaQueries(MediaQuerySet::create(m_media));

    m_sheet = cssSheet.release();

    // We don't need the cross-origin security check here because we are
    // getting the sheet text in "strict" mode. This enforces a valid CSS MIME
    // type.
    parseStyleSheet(sheet->sheetText());
#endif
}

void ProcessingInstruction::setXSLStyleSheet(const String& href, const KURL& baseURL, const String& sheet)
{
    if (!inDocument()) {
        ASSERT(!m_sheet);
        return;
    }

    assert(false); // Not reached!
}

void ProcessingInstruction::parseStyleSheet(const String& sheet)
{
#ifdef BLINKIT_CRAWLER_ONLY
    assert(false); // BKTODO: Not reached!
#else
    if (m_isCSS)
        toCSSStyleSheet(m_sheet.get())->contents()->parseString(sheet);

    clearResource();
    m_loading = false;

    if (m_isCSS)
        toCSSStyleSheet(m_sheet.get())->contents()->checkLoaded();
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
    if (isValid)
        process(href, charset);
    return InsertionDone;
}

void ProcessingInstruction::removedFrom(ContainerNode* insertionPoint)
{
#ifdef BLINKIT_CRAWLER_ONLY
    assert(false); // BKTODO: Not reached!
#else
    CharacterData::removedFrom(insertionPoint);
    if (!insertionPoint->inDocument())
        return;

    RefPtrWillBeRawPtr<StyleSheet> removedSheet = m_sheet;
    if (m_sheet) {
        ASSERT(m_sheet->ownerNode() == this);
        clearSheet();
    }

    // No need to remove pending sheets.
    clearResource();

    // If we're in document teardown, then we don't need to do any notification of our sheet's removal.
    if (document().isActive())
        document().removedStyleSheet(removedSheet.get());
#endif
}

void ProcessingInstruction::clearSheet()
{
#ifdef BLINKIT_CRAWLER_ONLY
    assert(false); // BKTODO: Not reached!
#else
    ASSERT(m_sheet);
    if (m_sheet->isLoading())
        document().styleEngine().removePendingSheet(this);
    m_sheet.release()->clearOwnerNode();
#endif
}

DEFINE_TRACE(ProcessingInstruction)
{
    visitor->trace(m_sheet);
    visitor->trace(m_listenerForXSLT);
    CharacterData::trace(visitor);
}

} // namespace
