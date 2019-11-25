// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: frame_fetch_context.cpp
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
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "frame_fetch_context.h"

#include "third_party/blink/renderer/core/loader/document_loader.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_fetcher.h"

namespace blink {

struct FrameFetchContext::FrozenState final : GarbageCollectedFinalized<FrozenState>
{
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FrameFetchContext::FrameFetchContext(DocumentLoader *loader, Document *document)
    : m_documentLoader(loader), m_document(document)
{
    assert(nullptr != GetFrame());
}

FrameFetchContext::~FrameFetchContext(void) = default;

std::unique_ptr<ResourceFetcher> FrameFetchContext::CreateFetcher(DocumentLoader *loader, Document *document)
{
    std::unique_ptr<FetchContext> context(new FrameFetchContext(loader, document));
    return ResourceFetcher::Create(context);
}

LocalFrame* FrameFetchContext::GetFrame(void) const
{
    assert(!IsDetached());

    if (!m_documentLoader)
    {
        assert(false); // BKTODO:
#if 0
        return FrameOfImportsController();
#endif
    }

    LocalFrame *frame = m_documentLoader->GetFrame();
    assert(nullptr != frame);
    return frame;
}

}  // namespace blink
