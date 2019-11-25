// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: frame_loader.cpp
// Description: FrameLoader Class
//      Author: Ziming Li
//     Created: 2019-09-14
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2006, 2007, 2008, 2009, 2010, 2011 Apple Inc. All rights
 * reserved.
 * Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies)
 * Copyright (C) 2008, 2009 Torch Mobile Inc. All rights reserved.
 * (http://www.torchmobile.com/)
 * Copyright (C) 2008 Alp Toker <alp@atoker.com>
 * Copyright (C) Research In Motion Limited 2009. All rights reserved.
 * Copyright (C) 2011 Kris Jordan <krisjordan@gmail.com>
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

#include "frame_loader.h"

#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/frame/local_frame_client.h"
#include "third_party/blink/renderer/core/loader/document_loader.h"
#include "third_party/blink/renderer/core/loader/navigation_scheduler.h"
#include "third_party/blink/renderer/platform/bindings/script_forbidden_scope.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_request.h"
#include "third_party/blink/renderer/platform/loader/fetch/substitute_data.h"

using namespace BlinKit;

namespace blink {

FrameLoader::FrameLoader(LocalFrame *frame) : m_frame(frame)
{
}

FrameLoader::~FrameLoader(void) = default;

LocalFrameClient* FrameLoader::Client(void) const
{
    return m_frame->Client();
}

void FrameLoader::CommitProvisionalLoad(void)
{
    if (!PrepareForCommit())
        return;

    Client()->TransitionToCommittedForNewPage();

    m_frame->GetNavigationScheduler().Cancel();
}

void FrameLoader::DispatchDidClearDocumentOfWindowObject(void)
{
    ASSERT(nullptr != m_frame->GetDocument());
    if (m_stateMachine.CreatingInitialEmptyDocument())
        return;
    ASSERT(false); // BKTODO:
}

void FrameLoader::FinishedParsing(void)
{
    if (m_stateMachine.CreatingInitialEmptyDocument())
        return;

    ASSERT(false); // BKTODO:
#if 0
    progress_tracker_->FinishedParsing();

    if (Client()) {
        ScriptForbiddenScope forbid_scripts;
        Client()->DispatchDidFinishDocumentLoad();
    }

    if (Client()) {
        Client()->RunScriptsAtDocumentReady(
            document_loader_ ? document_loader_->IsCommittedButEmpty() : true);
    }

    if (frame_->View()) {
        ProcessFragment(frame_->GetDocument()->Url(), document_loader_->LoadType(),
            kNavigationToDifferentDocument);
    }

    frame_->GetDocument()->CheckCompleted();
#endif
}

void FrameLoader::Init(void)
{
    ScriptForbiddenScope forbidScripts;

    BkURL emptyURL;
    ResourceRequest initialRequest(emptyURL);
    m_provisionalDocumentLoader = Client()->CreateDocumentLoader(m_frame, initialRequest, SubstituteData(), nullptr);
    m_provisionalDocumentLoader->StartLoading();

    m_frame->GetDocument()->CancelParsing();

    m_stateMachine.AdvanceTo(FrameLoaderStateMachine::kDisplayingInitialEmptyDocument);

    // Suppress finish notifications for initial empty documents, since they don't
    // generate start notifications.
    m_documentLoader->SetSentDidFinishLoad();
#if 0 // BKTODO: Check if necessary
    if (frame_->GetPage()->Paused())
        SetDefersLoading(true);
#endif
}

bool FrameLoader::PrepareForCommit(void)
{
    DocumentLoader *pdl = m_provisionalDocumentLoader.get();

    if (m_documentLoader)
    {
        ASSERT(false); // BKTODO:
    }

    // The previous calls to dispatchUnloadEvent() and detachChildren() can
    // execute arbitrary script via things like unload events. If the executed
    // script intiates a new load or causes the current frame to be detached, we
    // need to abandon the current load.
    if (pdl != m_provisionalDocumentLoader.get())
        return false;

    // detachFromFrame() will abort XHRs that haven't completed, which can trigger
    // event listeners for 'abort'. These event listeners might call
    // window.stop(), which will in turn detach the provisional document loader.
    // At this point, the provisional document loader should not detach, because
    // then the FrameLoader would not have any attached DocumentLoaders.
    if (m_documentLoader)
    {
        ASSERT(false); // BKTODO:
    }
    // 'abort' listeners can also detach the frame.
    if (nullptr == m_frame->Client())
        return false;
    assert(m_provisionalDocumentLoader.get() == pdl);

    // No more events will be dispatched so detach the Document.
    // TODO(yoav): Should we also be nullifying domWindow's document (or
    // domWindow) since the doc is now detached?
    if (Document *document = m_frame->GetDocument())
        document->Shutdown();
    m_documentLoader = std::move(m_provisionalDocumentLoader);
    if (m_documentLoader)
        m_documentLoader->MarkAsCommitted();
    return true;
}

void FrameLoader::StartNavigation(const FrameLoadRequest &request, WebFrameLoadType loadType)
{
    assert(WebFrameLoadType::kStandard == loadType || WebFrameLoadType::kReload == loadType);

    assert(nullptr != m_frame->GetDocument());
    assert(false); // BKTODO:
}

}  // namespace blink
