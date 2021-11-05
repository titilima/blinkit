// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: LinkResource.cpp
// Description: LinkResource Class
//      Author: Ziming Li
//     Created: 2021-07-24
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
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

#include "core/html/LinkResource.h"

#include "blinkit/ui/web_view_impl.h"
#include "core/HTMLNames.h"
#include "core/dom/Document.h"
#include "core/html/HTMLLinkElement.h"
#include "core/html/imports/HTMLImportsController.h"

namespace blink {

using namespace HTMLNames;

LinkResource::LinkResource(HTMLLinkElement* owner)
    : m_owner(owner)
{
}

LinkResource::~LinkResource()
{
}

bool LinkResource::shouldLoadResource() const
{
    return m_owner->document().frame() || m_owner->document().importsController();
}

LocalFrame* LinkResource::loadingFrame() const
{
    HTMLImportsController* importsController = m_owner->document().importsController();
    if (!importsController)
        return m_owner->document().frame();
    return importsController->master()->frame();
}

DEFINE_TRACE(LinkResource)
{
    // BKTODO: visitor->trace(m_owner);
}

LinkRequestBuilder::LinkRequestBuilder(HTMLLinkElement* owner)
    : m_owner(owner)
    , m_url(owner->getNonEmptyURLAttribute(hrefAttr))
{
    m_charset = m_owner->getAttribute(charsetAttr);
    if (m_charset.isEmpty() && m_owner->document().frame())
        m_charset = m_owner->document().characterSet();
}

FetchRequest LinkRequestBuilder::build(bool blocking) const
{
    // BKTODO: ResourceLoadPriority priority = blocking ? ResourceLoadPriorityUnresolved : ResourceLoadPriorityVeryLow;
    ResourceRequest resourceRequest(m_owner->document().completeURL(m_url.string()));
    return FetchRequest(resourceRequest, m_owner->localName(), m_charset);
}

} // namespace blink
