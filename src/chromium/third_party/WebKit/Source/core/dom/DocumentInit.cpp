// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: DocumentInit.cpp
// Description: DocumentInit Class
//      Author: Ziming Li
//     Created: 2019-02-06
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2001 Dirk Mueller (mueller@kde.org)
 *           (C) 2006 Alexey Proskuryakov (ap@webkit.org)
 * Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2012 Apple Inc. All rights reserved.
 * Copyright (C) 2008, 2009 Torch Mobile Inc. All rights reserved. (http://www.torchmobile.com/)
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies)
 * Copyright (C) 2013 Google Inc. All rights reserved.
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
 *
 */

#include "core/dom/DocumentInit.h"

#include "core/dom/Document.h"
#include "core/dom/custom/CustomElementRegistrationContext.h"
#include "core/frame/LocalFrame.h"
#include "core/html/imports/HTMLImportsController.h"
#include "core/loader/DocumentLoader.h"
#include "platform/RuntimeEnabledFeatures.h"
#include "public/platform/Platform.h"

namespace blink {

DocumentInit::DocumentInit(const KURL& url, LocalFrame* frame, WeakPtrWillBeRawPtr<Document> contextDocument, HTMLImportsController* importsController)
    : m_url(url)
    , m_frame(frame)
    , m_contextDocument(contextDocument)
    , m_shouldReuseDefaultView(frame && frame->shouldReuseDefaultView(url))
{
#ifndef BLINKIT_CRAWLER_ONLY
    if (m_frame->IsCrawlerFrame())
        ASSERT(nullptr == importsController);
    else
        m_importsController = importsController;
#endif
}

DocumentInit::DocumentInit(const DocumentInit&) = default;

DocumentInit::~DocumentInit()
{
}

bool DocumentInit::shouldSetURL() const
{
#ifdef BLINKIT_CRAWLER_ONLY
    return !m_url.isEmpty();
#else
    // BKTODO: Disable frame owner!
    LocalFrame* frame = frameForSecurityContext();
    return (frame && frame->owner()) || !m_url.isEmpty();
#endif
}

LocalFrame* DocumentInit::frameForSecurityContext() const
{
    if (m_frame)
        return m_frame;
#ifndef BLINKIT_CRAWLER_ONLY
    if (!m_frame->IsCrawlerFrame() && m_importsController)
        return m_importsController->master()->frame();
#endif
    return 0;
}

bool DocumentInit::shouldEnforceStrictMixedContentChecking() const
{
    ASSERT(frameForSecurityContext());
    return frameForSecurityContext()->loader().shouldEnforceStrictMixedContentChecking();
}

SecurityContext::InsecureRequestsPolicy DocumentInit::insecureRequestsPolicy() const
{
    ASSERT(frameForSecurityContext());
    return frameForSecurityContext()->loader().insecureRequestsPolicy();
}

SecurityContext::InsecureNavigationsSet* DocumentInit::insecureNavigationsToUpgrade() const
{
    ASSERT(frameForSecurityContext());
    return frameForSecurityContext()->loader().insecureNavigationsToUpgrade();
}

bool DocumentInit::isHostedInReservedIPRange() const
{
    if (LocalFrame* frame = frameForSecurityContext()) {
        if (DocumentLoader* loader = frame->loader().provisionalDocumentLoader() ? frame->loader().provisionalDocumentLoader() : frame->loader().documentLoader()) {
            if (!loader->response().remoteIPAddress().isEmpty())
                return Platform::current()->isReservedIPAddress(loader->response().remoteIPAddress());
        }
    }
    return false;
}

Settings* DocumentInit::settings() const
{
    ASSERT(frameForSecurityContext());
    return frameForSecurityContext()->settings();
}

#ifndef BLINKIT_CRAWLER_ONLY
DocumentInit& DocumentInit::withRegistrationContext(CustomElementRegistrationContext* registrationContext)
{
    ASSERT(!m_createNewRegistrationContext && !m_registrationContext);
    m_registrationContext = registrationContext;
    return *this;
}

DocumentInit& DocumentInit::withNewRegistrationContext()
{
    ASSERT(!m_createNewRegistrationContext && !m_registrationContext);
    m_createNewRegistrationContext = true;
    return *this;
}

PassRefPtrWillBeRawPtr<CustomElementRegistrationContext> DocumentInit::registrationContext(Document* document) const
{
    if (!document->isHTMLDocument() && !document->isXHTMLDocument())
        return nullptr;

    if (m_createNewRegistrationContext)
        return CustomElementRegistrationContext::create();

    return m_registrationContext.get();
}
#endif // BLINKIT_CRAWLER_ONLY

WeakPtrWillBeRawPtr<Document> DocumentInit::contextDocument() const
{
    return m_contextDocument;
}

DocumentInit DocumentInit::fromContext(WeakPtrWillBeRawPtr<Document> contextDocument, const KURL& url)
{
    return DocumentInit(url, 0, contextDocument, 0);
}

} // namespace blink

