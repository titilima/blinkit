// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: location.cpp
// Description: Location Class
//      Author: Ziming Li
//     Created: 2020-03-26
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2008, 2010 Apple Inc. All rights reserved.
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

#include "location.h"

#include "blinkit/common/bk_url.h"
#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/frame/local_dom_window.h"

using namespace BlinKit;

namespace blink {

Location::Location(DOMWindow *domWindow) : m_domWindow(domWindow)
{
}

Document* Location::GetDocument(void) const
{
    return ToLocalDOMWindow(m_domWindow)->document();
}

std::string Location::host(void) const
{
    return Url().Host();
}

std::string Location::href(void) const
{
    return Url().StrippedForUseAsReferrer();
}
std::string Location::protocol(void) const
{
    return Url().Scheme();
}

const BkURL& Location::Url(void) const
{
    const BkURL& url = GetDocument()->Url();
    if (!url.IsValid())
    {
        // Use "about:blank" while the page is still loading (before we have a
        // frame).
        return BkURL::Blank();
    }

    return url;
}

} // namespace blink
