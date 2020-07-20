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

#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/frame/local_dom_window.h"
#include "third_party/blink/renderer/platform/weborigin/kurl.h"

namespace blink {

Location::Location(DOMWindow *domWindow) : m_domWindow(domWindow)
{
}

Document* Location::GetDocument(void) const
{
    return ToLocalDOMWindow(m_domWindow)->document();
}

std::string Location::hash(void) const
{
    std::string ret;
    const GURL &u = Url();
    if (u.has_ref())
    {
        ret.push_back('#');
        ret.append(u.ref());
    }
    return ret;
}

std::string Location::host(void) const
{
    return Url().host();
}

std::string Location::href(void) const
{
    return Url().GetAsReferrer().spec();
}

std::string Location::pathname(void) const
{
    return Url().path();
}

std::string Location::protocol(void) const
{
    return Url().scheme();
}

std::string Location::search(void) const
{
    std::string ret;
    std::string query = Url().query();
    if (!query.empty())
    {
        ret.push_back('?');
        ret.append(query);
    }
    return ret;
}

const GURL& Location::Url(void) const
{
    const GURL& url = GetDocument()->Url();
    if (!url.is_valid())
    {
        // Use "about:blank" while the page is still loading (before we have a
        // frame).
        return BlankURL();
    }

    return url;
}

} // namespace blink
