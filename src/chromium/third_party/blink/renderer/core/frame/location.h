// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: location.h
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

#ifndef BLINKIT_BLINK_LOCATION_H
#define BLINKIT_BLINK_LOCATION_H

#pragma once

#include "base/memory/ptr_util.h"
#include "third_party/blink/renderer/platform/bindings/script_wrappable.h"
#include "third_party/blink/renderer/platform/wtf/text/wtf_string.h"

class GURL;

namespace blink {

class DOMWindow;
class Document;
class ExceptionState;

// This class corresponds to the Location interface. Location is the only
// interface besides Window that is accessible cross-origin and must handle
// remote frames.
//
// HTML standard: https://whatwg.org/C/browsers.html#the-location-interface
class Location final : public ScriptWrappable
{
public:
    static std::unique_ptr<Location> Create(DOMWindow *domWindow)
    {
        return base::WrapUnique(new Location(domWindow));
    }

    // Exports for JS
    std::string hash(void) const;
    std::string host(void) const;
    std::string href(void) const;
    std::string pathname(void) const;
    std::string protocol(void) const;
    std::string search(void) const;
private:
    explicit Location(DOMWindow *domWindow);

    // Note: it is only valid to call this if this is a Location object for a
    // LocalDOMWindow.
    Document* GetDocument(void) const;

    const GURL& Url(void) const;

    const Member<DOMWindow> m_domWindow;
};

} // namespace blink

#endif // BLINKIT_BLINK_LOCATION_H
