// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: frame_fetch_context.h
// Description: FrameFetchContext Class
//      Author: Ziming Li
//     Created: 2019-10-10
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2013 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
n * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef BLINKIT_BLINK_FRAME_FETCH_CONTEXT_H
#define BLINKIT_BLINK_FRAME_FETCH_CONTEXT_H

#pragma once

#include "third_party/blink/renderer/core/loader/base_fetch_context.h"
#include "third_party/blink/renderer/platform/heap/handle.h"

namespace blink {

class Document;
class DocumentLoader;
class LocalFrame;
class LocalFrameClient;
class ResourceFetcher;

class FrameFetchContext final : public BaseFetchContext
{
public:
    ~FrameFetchContext(void) override;
    void Trace(Visitor *visitor) override;

    static ResourceFetcher* CreateFetcherFromDocumentLoader(DocumentLoader *loader)
    {
        return CreateFetcher(loader, nullptr);
    }
    static void ProvideDocumentToContext(FetchContext &context, Document *document);

    bool IsDetached(void) const override { return !!m_frozenState; }
private:
    FrameFetchContext(DocumentLoader *loader, Document *document);

    static ResourceFetcher* CreateFetcher(DocumentLoader *loader, Document *document);

    LocalFrame* GetFrame(void) const;
    LocalFrameClient* GetLocalFrameClient(void) const;
    void SetFirstPartyCookie(ResourceRequest &request);
    String GetUserAgent(void) const;

    // FetchContext overrides
    bool IsFrameFetchContext(void) override { return true; }
    void RecordDataUriWithOctothorpe(void) override;
    void PrepareRequest(ResourceRequest &request, RedirectType redirectType) override;
    bool ShouldLoadNewResource(ResourceType type) const override;
    void DispatchDidReceiveResponse(unsigned long identifier, const ResourceResponse &response,
        Resource *resource) override;
    void DidLoadResource(Resource *resource) override;
    FetchContext* Detach(void) override;
    // BaseFetchContext overrides
    const FetchClientSettingsObject* GetFetchClientSettingsObject(void) const override { return m_fetchClientSettingsObject.get(); }

    Member<DocumentLoader> m_documentLoader;
    Member<Document> m_document;

    struct FrozenState;
    std::unique_ptr<const FrozenState> m_frozenState; // Non-null only when detached.

    std::unique_ptr<FetchClientSettingsObject> m_fetchClientSettingsObject;
};

}  // namespace blink

#endif // BLINKIT_BLINK_FRAME_FETCH_CONTEXT_H
