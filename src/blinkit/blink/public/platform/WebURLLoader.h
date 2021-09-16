// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: WebURLLoader.h
// Description: WebURLLoader Class
//      Author: Ziming Li
//     Created: 2021-06-25
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2009, 2011 Google Inc. All rights reserved.
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
 */

#ifndef WebURLLoader_h
#define WebURLLoader_h

#include "WebCommon.h"
// BKTODO: #include "WebURLRequest.h"

namespace blink {

class ResourceRequest;
// BKTODO: class WebData;
class WebTaskRunner;
// BKTODO: class WebThreadedDataReceiver;
class WebURLLoaderClient;
#if 0 // BKTODO:
class WebURLResponse;
struct WebURLError;
#endif

class WebURLLoader {
public:
    // The WebURLLoader may be deleted in a call to its client.
    virtual ~WebURLLoader() {}

#if 0 // BKTODO:
    // Load the request synchronously, returning results directly to the
    // caller upon completion.  There is no mechanism to interrupt a
    // synchronous load!!
    virtual void loadSynchronously(const WebURLRequest&,
        WebURLResponse&, WebURLError&, WebData& data) = 0;
#endif

    // Load the request asynchronously, sending notifications to the given
    // client.  The client will receive no further notifications if the
    // loader is disposed before it completes its work.
    virtual void loadAsynchronously(const ResourceRequest&,
        WebURLLoaderClient*) = 0;

    // Cancels an asynchronous load.  This will appear as a load error to
    // the client.
    virtual void cancel() = 0;

#if 0 // BKTODO:
    // Suspends/resumes an asynchronous load.
    virtual void setDefersLoading(bool) = 0;

    // Notifies the loader that the priority of a WebURLRequest has changed from
    // its previous value. For example, a preload request starts with low
    // priority, but may increase when the resource is needed for rendering.
    virtual void didChangePriority(WebURLRequest::Priority newPriority) { }
    virtual void didChangePriority(WebURLRequest::Priority newPriority, int intraPriorityValue) { didChangePriority(newPriority); }

    // Try to attach the given data receiver which from this point will receive data
    // on its thread (provided by WebThreadedDataReceiver::backgroundThread(),
    // rather than the WebURLLoaderClient. If successful, ownership
    // of the data receiver is assumed by the WebURLLoader and the receiver should
    // be deleted on the main thread when no longer needed.
    virtual bool attachThreadedDataReceiver(WebThreadedDataReceiver*) { return false; }
#endif

    // Sets the task runner for which any loading tasks should be posted on.
    // Takes ownership of the WebTaskRunner.
    virtual void setLoadingTaskRunner(const std::shared_ptr<WebTaskRunner>&) = 0;
};

} // namespace blink

#endif
