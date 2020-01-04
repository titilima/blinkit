// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: web_url_loader_client.h
// Description: WebURLLoaderClient Class
//      Author: Ziming Li
//     Created: 2019-12-11
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2009 Google Inc. All rights reserved.
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

#ifndef BLINKIT_BLINK_WEB_URL_LOADER_CLIENT_H
#define BLINKIT_BLINK_WEB_URL_LOADER_CLIENT_H

#pragma once

namespace blink {

class ResourceResponse;

class WebURLLoaderClient
{
public:
    // Called when response headers are received.
    virtual void DidReceiveResponse(const ResourceResponse &response) = 0;

    // Called when a chunk of response data is received. |data_length| is the
    // number of bytes pointed to by |data|. |encoded_data_length| is the number
    // of bytes actually received from network to serve this chunk, including
    // HTTP headers and framing if relevant. It is 0 if the response was served
    // from cache, and -1 if this information is unavailable.
    virtual void DidReceiveData(const char *data, int dataLength) {}

    // Called when the load completes successfully.
    virtual void DidFinishLoading(void) {}
protected:
    virtual ~WebURLLoaderClient(void) = default;
};

} // namespace blink

#endif // BLINKIT_BLINK_WEB_URL_LOADER_CLIENT_H
