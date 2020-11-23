// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: document_loader.h
// Description: DocumentLoader Class
//      Author: Ziming Li
//     Created: 2019-09-28
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2006, 2007, 2008, 2009 Apple Inc. All rights reserved.
 * Copyright (C) 2011 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef BLINKIT_BLINK_DOCUMENT_LOADER_H
#define BLINKIT_BLINK_DOCUMENT_LOADER_H

#pragma once

#include "third_party/blink/public/web/web_frame_load_type.h"
#include "third_party/blink/public/web/web_global_object_reuse_policy.h"
#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/loader/fetch/raw_resource.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_request.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_response.h"
#include "third_party/blink/renderer/platform/loader/fetch/substitute_data.h"
#include "third_party/blink/renderer/platform/wtf/time.h"

namespace blink {

class Document;
class DocumentParser;
class FrameLoader;
class LocalFrame;
class LocalFrameClient;
class ResourceError;
class ResourceFetcher;

class DocumentLoader : public GarbageCollectedFinalized<DocumentLoader>, private RawResourceClient
{
public:
    BK_DECLARE_GC_NAME(DocumentLoader)

    ~DocumentLoader(void) override;
    void Trace(Visitor *visitor) override;

    LocalFrame* GetFrame(void) const { return m_frame; }
    ResourceFetcher* Fetcher(void) const { return m_fetcher.Get(); }
    const ResourceResponse& GetResponse(void) const { return m_response; }
    const GURL& Url(void) const { return m_currentRequest.Url(); }
    WebFrameLoadType LoadType(void) const { return m_loadType; }
    void SetLoadType(WebFrameLoadType loadType) { m_loadType = loadType; }

    void AppendRedirect(const GURL &url) { m_redirectChain.push_back(url); }

    // Without PlzNavigate, this is only false for a narrow window during
    // navigation start. For PlzNavigate, a navigation sent to the browser will
    // leave a dummy DocumentLoader in the NotStarted state until the navigation
    // is actually handled in the renderer.
    bool DidStart(void) const { return m_state != kNotStarted; }
    void StartLoading(void);
    void StopLoading(void);
    void MarkAsCommitted(void);
    bool SentDidFinishLoad(void) const { return kSentDidFinishLoad == m_state; }
    void SetSentDidFinishLoad(void) { m_state = kSentDidFinishLoad; }
    void LoadFailed(const ResourceError &error);

    virtual void DetachFromFrame(bool flushMicrotaskQueue);
protected:
    DocumentLoader(LocalFrame *frame, const ResourceRequest &request, const SubstituteData &substituteData);

    std::vector<GURL> m_redirectChain;
private:
    FrameLoader& GetFrameLoader(void) const;
    LocalFrameClient& GetLocalFrameClient(void) const;

    // installNewDocument() does the work of creating a Document and
    // DocumentParser, as well as creating a new LocalDOMWindow if needed. It also
    // initalizes a bunch of state on the Document (e.g., the state based on
    // response headers).
    void InstallNewDocument(const GURL &URL, WebGlobalObjectReusePolicy reusePolicy,
        const AtomicString &mimeType, const AtomicString &encoding, const GURL &overridingURL);
    void DidInstallNewDocument(Document *document);

    void WillCommitNavigation(void);
    void CommitNavigation(const AtomicString &mimeType, const GURL &overridingURL = GURL());
    void DidCommitNavigation(WebGlobalObjectReusePolicy);
    void CommitData(const char *bytes, size_t length);
    void FinishedLoading(void);
    bool MaybeLoadEmpty(void);
    void ProcessData(const char *data, size_t length);
    // Processes the data stored in the m_dataBuffer, used to avoid appending data
    // to the parser in a nested message loop.
    void ProcessDataBuffer();

    bool ShouldContinueForResponse(void) const;

    // ResourceClient overrides
    void DataReceived(Resource *resource, const char *data, size_t length) final;
    void NotifyFinished(Resource *resource) final;
    // RawResourceClient overrides
    void ResponseReceived(Resource *resource, const ResourceResponse &response) final;

    Member<LocalFrame> m_frame;
    Member<ResourceFetcher> m_fetcher;
    Member<DocumentParser> m_parser;
    ResourceRequest m_originalRequest, m_currentRequest;
    SubstituteData m_substituteData;
    ResourceResponse m_response;
    WebFrameLoadType m_loadType = WebFrameLoadType::kStandard;

    enum State { kNotStarted, kProvisional, kCommitted, kSentDidFinishLoad };
    State m_state = kNotStarted;

    bool m_dataReceived = false;
    // Used to protect against reentrancy into dataReceived().
    bool m_inDataReceived = false;
    std::shared_ptr<SharedBuffer> m_dataBuffer;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_LOADER_DOCUMENT_LOADER_H_
