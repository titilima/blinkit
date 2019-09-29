// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: document_loader.cpp
// Description: DocumentLoader Class
//      Author: Ziming Li
//     Created: 2019-09-28
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2006, 2007, 2008 Apple Inc. All rights reserved.
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

#include "document_loader.h"

#include "third_party/blink/renderer/core/frame/local_frame.h"

namespace blink {

DocumentLoader::DocumentLoader(LocalFrame *frame, const ResourceRequest &request, const SubstituteData &substituteData)
    : m_frame(frame)
    , m_originalRequest(request), m_currentRequest(request)
    , m_substituteData(substituteData)
{
}

FrameLoader& DocumentLoader::GetFrameLoader(void) const
{
    DCHECK(m_frame);
    return m_frame->Loader();
}

bool DocumentLoader::MaybeLoadEmpty(void)
{
    bool shouldLoadEmpty = !m_substituteData.IsValid() && m_currentRequest.Url().IsEmpty();
    if (!shouldLoadEmpty)
        return false;

    if (m_currentRequest.Url().IsEmpty() &&
        !GetFrameLoader().StateMachine()->CreatingInitialEmptyDocument())
    {
        request_.SetURL(BlankURL());
    }
    response_ = ResourceResponse(request_.Url());
    response_.SetMimeType("text/html");
    response_.SetTextEncodingName("utf-8");
    FinishedLoading(CurrentTimeTicks());
    return true;
}

void DocumentLoader::StartLoading(void)
{
    DCHECK(!GetResource());
    DCHECK_EQ(m_state, kNotStarted);
    m_state = kProvisional;

    if (MaybeLoadEmpty())
        return;

    assert(false); // BKTODO:
}

}  // namespace blink
