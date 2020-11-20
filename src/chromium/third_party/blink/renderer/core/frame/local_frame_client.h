// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: local_frame_client.h
// Description: LocalFrameClient Class
//      Author: Ziming Li
//     Created: 2019-09-13
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2006, 2007, 2008, 2009, 2010, 2011, 2012 Apple Inc. All rights
 * reserved.
 * Copyright (C) 2012 Google Inc. All rights reserved.
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

#ifndef BLINKIT_BLINK_LOCAL_FRAME_CLIENT_H
#define BLINKIT_BLINK_LOCAL_FRAME_CLIENT_H

#pragma once

#include "third_party/blink/public/web/web_document_loader.h"
#include "third_party/blink/renderer/core/frame/frame_client.h"
#include "third_party/blink/renderer/core/loader/navigation_policy.h"
#include "third_party/blink/renderer/platform/wtf/text/wtf_string.h"

namespace blink {

class DocumentLoader;
class LocalFrame;
class ResourceError;
class ResourceRequest;
class SubstituteData;

class LocalFrameClient : public FrameClient
{
public:
    virtual bool IsCrawler(void) const = 0;
    virtual bool HasWebView(void) const = 0;  // mainly for assertions

    virtual DocumentLoader* CreateDocumentLoader(LocalFrame *frame, const ResourceRequest &request,
        const SubstituteData &data, std::unique_ptr<WebDocumentLoader::ExtraData> extraData) = 0;

    virtual String UserAgent(void) = 0;

    virtual void TransitionToCommittedForNewPage(void) = 0;
    
    virtual void DispatchWillSendRequest(ResourceRequest &request) = 0;

    virtual void DidCreateNewDocument(void) = 0;
    virtual void DispatchDidReceiveTitle(const String &title) = 0;
    virtual void DispatchDidFailProvisionalLoad(const ResourceError &error) = 0;
    virtual void DispatchDidFailLoad(const ResourceError &error) = 0;
    virtual void RunScriptsAtDocumentIdle(void) {}
    virtual void DispatchDidFinishDocumentLoad(void) = 0;
    virtual void DispatchDidHandleOnloadEvents(void) = 0;
    virtual void DispatchDidFinishLoad(void) = 0;
    virtual void WillBeDetached(void) = 0;

    virtual NavigationPolicy DecidePolicyForNavigation(const ResourceRequest &request) { return kNavigationPolicyCurrentTab; }
    virtual void DispatchDidStartProvisionalLoad(DocumentLoader *loader, ResourceRequest &request) {}
};

}  // namespace blink

#endif  // BLINKIT_BLINK_LOCAL_FRAME_CLIENT_H
