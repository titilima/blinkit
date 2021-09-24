// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: LinkLoader.h
// Description: LinkLoader Class
//      Author: Ziming Li
//     Created: 2021-07-15
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2011 Google Inc. All rights reserved.
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
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef LinkLoader_h
#define LinkLoader_h

#include "core/CoreExport.h"
#include "core/fetch/ResourceClient.h"
#include "core/fetch/ResourceOwner.h"
#include "core/loader/LinkLoaderClient.h"
#if 0 // BKTODO:
#include "platform/CrossOriginAttributeValue.h"
#include "platform/PrerenderClient.h"
#endif
#include "platform/Timer.h"
#include "platform/heap/Handle.h"
#include "wtf/OwnPtr.h"

namespace blink {

class Document;
class LinkRelAttribute;
// BKTODO: class NetworkHintsInterface;
class PrerenderHandle;

// The LinkLoader can load link rel types icon, dns-prefetch, subresource, prefetch and prerender.
class CORE_EXPORT LinkLoader final : public ResourceOwner<Resource, ResourceClient> { // BKTODO:, public PrerenderClient {
    DISALLOW_NEW();
public:
    explicit LinkLoader(LinkLoaderClient*);
    ~LinkLoader() override;

    // from ResourceClient
    void notifyFinished(Resource*) override;
    String debugName() const override { return "LinkLoader"; }

#if 0 // BKTODO:
    // from PrerenderClient
    void didStartPrerender() override;
    void didStopPrerender() override;
    void didSendLoadForPrerender() override;
    void didSendDOMContentLoadedForPrerender() override;
#endif

    void released();
    bool loadLink(const LinkRelAttribute&, const String& type, const KURL&, Document&);
    enum CanLoadResources { LoadResources, DoNotLoadResources };
    // BKTODO: static bool loadLinkFromHeader(const String& headerValue, Document*, const NetworkHintsInterface&, CanLoadResources);
    static Resource::Type getTypeFromAsAttribute(const String& as, Document*);

    DECLARE_TRACE();

private:
    void linkLoadTimerFired(Timer<LinkLoader>*);
    void linkLoadingErrorTimerFired(Timer<LinkLoader>*);

    LinkLoaderClient* m_client;

    Timer<LinkLoader> m_linkLoadTimer;
    Timer<LinkLoader> m_linkLoadingErrorTimer;

    // BKTODO: OwnPtrWillBeMember<PrerenderHandle> m_prerender;
};

}

#endif
