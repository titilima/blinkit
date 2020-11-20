// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: web_document_loader_impl.h
// Description: WebDocumentLoaderImpl Class
//      Author: Ziming Li
//     Created: 2019-09-28
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

#ifndef BLINKIT_BLINK_WEB_DOCUMENT_LOADER_IMPL_H
#define BLINKIT_BLINK_WEB_DOCUMENT_LOADER_IMPL_H

#include "third_party/blink/public/web/web_document_loader.h"
#include "third_party/blink/renderer/core/loader/document_loader.h"

namespace blink {

class WebDocumentLoaderImpl final : public DocumentLoader, public WebDocumentLoader
{
public:
    static WebDocumentLoaderImpl* Create(LocalFrame *frame, const ResourceRequest &request,
        const SubstituteData &substituteData);

    // WebDocumentLoader
    ExtraData* GetExtraData(void) const override { return m_extraData.get(); }
    void SetExtraData(std::unique_ptr<ExtraData> extraData) override { m_extraData = std::move(extraData); }
private:
    WebDocumentLoaderImpl(LocalFrame *frame, const ResourceRequest &request, const SubstituteData &substituteData);

#ifndef BLINKIT_CRAWLER_ONLY
    String DebugName(void) const override { return "WebDocumentLoaderImpl"; }
#endif

    std::unique_ptr<ExtraData> m_extraData;
};

}  // namespace blink

#endif  // BLINKIT_BLINK_WEB_DOCUMENT_LOADER_IMPL_H
