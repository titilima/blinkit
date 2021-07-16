// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: FrameLoadRequest.h
// Description: FrameLoadRequest Struct
//      Author: Ziming Li
//     Created: 2021-07-16
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2003, 2006, 2010 Apple Inc. All rights reserved.
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

#ifndef FrameLoadRequest_h
#define FrameLoadRequest_h

#include "core/dom/Document.h"
#include "core/events/Event.h"
#include "core/fetch/ResourceLoaderOptions.h"
#include "core/fetch/SubstituteData.h"
#include "core/html/HTMLFormElement.h"
// BKTODO: #include "core/loader/FrameLoaderTypes.h"
#include "platform/network/ResourceRequest.h"

namespace blink {

struct FrameLoadRequest {
    STACK_ALLOCATED();
public:
    explicit FrameLoadRequest(Document* originDocument)
        : FrameLoadRequest(originDocument, ResourceRequest())
    {
    }

    FrameLoadRequest(Document* originDocument, const ResourceRequest& resourceRequest)
        : FrameLoadRequest(originDocument, resourceRequest, AtomicString())
    {
    }

    FrameLoadRequest(Document* originDocument, const ResourceRequest& resourceRequest, const AtomicString& frameName)
        : FrameLoadRequest(originDocument, resourceRequest, frameName, CheckContentSecurityPolicy)
    {
    }

    FrameLoadRequest(Document* originDocument, const ResourceRequest& resourceRequest, const SubstituteData& substituteData)
        : FrameLoadRequest(originDocument, resourceRequest, AtomicString(), substituteData, CheckContentSecurityPolicy)
    {
    }

    FrameLoadRequest(Document* originDocument, const ResourceRequest& resourceRequest, const AtomicString& frameName, ContentSecurityPolicyDisposition shouldCheckMainWorldContentSecurityPolicy)
        : FrameLoadRequest(originDocument, resourceRequest, frameName, SubstituteData(), shouldCheckMainWorldContentSecurityPolicy)
    {
    }

    Document* originDocument() const { return m_originDocument.get(); }

    ResourceRequest& resourceRequest() { return m_resourceRequest; }
    const ResourceRequest& resourceRequest() const { return m_resourceRequest; }

    const AtomicString& frameName() const { return m_frameName; }
    void setFrameName(const AtomicString& frameName) { m_frameName = frameName; }

    const SubstituteData& substituteData() const { return m_substituteData; }

    bool replacesCurrentItem() const { return m_replacesCurrentItem; }
    void setReplacesCurrentItem(bool replacesCurrentItem) { m_replacesCurrentItem = replacesCurrentItem; }

#if 0 // BKTODO:
    ClientRedirectPolicy clientRedirect() const { return m_clientRedirect; }
    void setClientRedirect(ClientRedirectPolicy clientRedirect) { m_clientRedirect = clientRedirect; }
#endif

    Event* triggeringEvent() const { return m_triggeringEvent.get(); }
    void setTriggeringEvent(PassRefPtrWillBeRawPtr<Event> triggeringEvent) { m_triggeringEvent = triggeringEvent; }

    HTMLFormElement* form() const { return m_form.get(); }
    void setForm(PassRefPtrWillBeRawPtr<HTMLFormElement> form) { m_form = form; }

#if 0 // BKTODO:
    ShouldSendReferrer shouldSendReferrer() const { return m_shouldSendReferrer; }
    void setShouldSendReferrer(ShouldSendReferrer shouldSendReferrer) { m_shouldSendReferrer = shouldSendReferrer; }

    ShouldSetOpener shouldSetOpener() const { return m_shouldSetOpener; }
    void setShouldSetOpener(ShouldSetOpener shouldSetOpener) { m_shouldSetOpener = shouldSetOpener; }
#endif

    ContentSecurityPolicyDisposition shouldCheckMainWorldContentSecurityPolicy() const { return m_shouldCheckMainWorldContentSecurityPolicy; }

private:
    FrameLoadRequest(Document* originDocument, const ResourceRequest& resourceRequest, const AtomicString& frameName, const SubstituteData& substituteData, ContentSecurityPolicyDisposition shouldCheckMainWorldContentSecurityPolicy)
        : m_originDocument(originDocument)
        , m_resourceRequest(resourceRequest)
        , m_frameName(frameName)
        , m_substituteData(substituteData)
        , m_replacesCurrentItem(false)
#if 0 // BKTODO:
        m_clientRedirect(NotClientRedirect)
        , m_shouldSendReferrer(MaybeSendReferrer)
        , m_shouldSetOpener(MaybeSetOpener)
#endif
        , m_shouldCheckMainWorldContentSecurityPolicy(shouldCheckMainWorldContentSecurityPolicy)
    {
        initializeFetchFlags();
#if 0 // BKTODO:
        if (originDocument)
            m_resourceRequest.setRequestorOrigin(SecurityOrigin::create(originDocument->url()));
#endif
    }
    void initializeFetchFlags()
    {
        ASSERT(false); // BKTODO:
#if 0
        // These flags are passed to a service worker which controls the page.
        m_resourceRequest.setFetchRequestMode(WebURLRequest::FetchRequestModeNavigate);
        m_resourceRequest.setFetchCredentialsMode(WebURLRequest::FetchCredentialsModeInclude);
        m_resourceRequest.setFetchRedirectMode(WebURLRequest::FetchRedirectModeManual);
#endif
    }
    RefPtrWillBeMember<Document> m_originDocument;
    ResourceRequest m_resourceRequest;
    AtomicString m_frameName;
    SubstituteData m_substituteData;
    bool m_replacesCurrentItem;
    // BKTODO: ClientRedirectPolicy m_clientRedirect;
    RefPtrWillBeMember<Event> m_triggeringEvent;
    RefPtrWillBeMember<HTMLFormElement> m_form;
#if 0 // BKTODO:
    ShouldSendReferrer m_shouldSendReferrer;
    ShouldSetOpener m_shouldSetOpener;
#endif
    ContentSecurityPolicyDisposition m_shouldCheckMainWorldContentSecurityPolicy;
};

}

#endif // FrameLoadRequest_h
