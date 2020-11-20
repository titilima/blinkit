// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: frame_loader.h
// Description: FrameLoader Class
//      Author: Ziming Li
//     Created: 2019-09-14
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2006, 2007, 2008, 2009, 2011 Apple Inc. All rights reserved.
 * Copyright (C) 2008, 2009 Torch Mobile Inc. All rights reserved.
 * (http://www.torchmobile.com/)
 * Copyright (C) Research In Motion Limited 2009. All rights reserved.
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

#ifndef BLINKIT_BLINK_FRAME_LOADER_H
#define BLINKIT_BLINK_FRAME_LOADER_H

#pragma once

#include "third_party/blink/public/web/web_document_loader.h"
#include "third_party/blink/public/web/web_frame_load_type.h"
#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/core/loader/frame_loader_state_machine.h"

class GURL;

namespace blink {

class DocumentLoader;
class FrameLoaderStateMachine;
struct FrameLoadRequest;
class LocalFrame;
class LocalFrameClient;
class ResourceRequest;
class SubstituteData;

class FrameLoader final
{
    DISALLOW_NEW();
public:
    explicit FrameLoader(LocalFrame *frame);
    ~FrameLoader(void);
    void Trace(Visitor *visitor);

    FrameLoaderStateMachine* StateMachine(void) const { return &m_stateMachine; }
    DocumentLoader* GetDocumentLoader(void) const { return m_documentLoader.Get(); }
    DocumentLoader* GetProvisionalDocumentLoader(void) const { return m_provisionalDocumentLoader.Get(); }
    // Note: When a PlzNavigtate navigation is handled by the client, we will
    // have created a dummy provisional DocumentLoader, so this will return true
    // while the client handles the navigation.
    bool HasProvisionalNavigation(void) const { return nullptr != GetProvisionalDocumentLoader(); }

    String UserAgent(void) const;

    void Init(void);
    void StartNavigation(const FrameLoadRequest &passedRequest, WebFrameLoadType loadType = WebFrameLoadType::kStandard);
    void CommitNavigation(const ResourceRequest &request, const SubstituteData &substituteData,
        WebFrameLoadType loadType = WebFrameLoadType::kStandard,
        std::unique_ptr<WebDocumentLoader::ExtraData> extraData = nullptr);
    bool PrepareForCommit(void);
    void CommitProvisionalLoad(void);
    void FinishedParsing(void);
    void DidFinishNavigation(void);
    void DetachProvisionalDocumentLoader(DocumentLoader *loader);
    // This runs the "stop document loading" algorithm in HTML:
    // https://html.spec.whatwg.org/C/browsing-the-web.html#stop-document-loading
    // Note, this function only cancels ongoing navigation handled through
    // FrameLoader. You might also want to call
    // LocalFrameClient::AbortClientNavigation() if appropriate.
    //
    // Warning: StopAllLoaders() may detach the LocalFrame to which this
    // FrameLoader belongs. Callers need to be careful about checking the
    // existence of the frame after StopAllLoaders() returns.
    void StopAllLoaders(void);
    void Detach(void);

    void DispatchDidClearDocumentOfWindowObject(void);
    void DispatchUnloadEvent(void);

    static void SetReferrerForFrameRequest(FrameLoadRequest &frameRequest);
private:
    LocalFrameClient* Client(void) const;

    DocumentLoader* CreateDocumentLoader(const ResourceRequest &request, const SubstituteData &substituteData,
        WebFrameLoadType loadType, std::unique_ptr<WebDocumentLoader::ExtraData> extraData);
    SubstituteData DefaultSubstituteDataForURL(const GURL &url);

    // Returns whether we should continue with new navigation.
    bool CancelProvisionalLoaderForNewNavigation(bool cancelScheduledNavigations);
    bool ShouldPerformFragmentNavigation(bool isFormSubmission, const String &httpMethod, WebFrameLoadType loadType,
        const GURL &url);
    void DetachDocumentLoader(Member<DocumentLoader> &loader, bool flushMicrotaskQueue = false);

    Member<LocalFrame> m_frame;
    mutable FrameLoaderStateMachine m_stateMachine;
    Member<DocumentLoader> m_documentLoader;
    Member<DocumentLoader> m_provisionalDocumentLoader;

    bool m_inStopAllLoaders = false;
    bool m_protectProvisionalLoader = false;
    bool m_detached = false;

    DISALLOW_COPY_AND_ASSIGN(FrameLoader);
};

}  // namespace blink

#endif  // BLINKIT_BLINK_FRAME_LOADER_H
