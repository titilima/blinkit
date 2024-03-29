// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: CSSImageValue.h
// Description: CSSImageValue Class
//      Author: Ziming Li
//     Created: 2021-07-14
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * (C) 1999-2003 Lars Knoll (knoll@kde.org)
 * Copyright (C) 2004, 2005, 2006, 2008, 2012 Apple Inc. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef CSSImageValue_h
#define CSSImageValue_h

#include "core/CoreExport.h"
#include "core/css/CSSValue.h"
#if 0 // BKTODO:
#include "platform/CrossOriginAttributeValue.h"
#include "platform/weborigin/Referrer.h"
#else
#include "blinkit/blink/renderer/wtf/text/WTFString.h"
#endif
#include "wtf/RefPtr.h"

namespace blink {

class Document;
class KURL;
class StyleFetchedImage;
class StyleImage;
class LayoutObject;

class CORE_EXPORT CSSImageValue : public CSSValue {
public:
    static GCRefPtr<CSSImageValue> create(const KURL& url, StyleFetchedImage* image = 0)
    {
        return create(String::fromStdUTF8(url.spec()), url, image);
    }
    static GCRefPtr<CSSImageValue> create(const String& rawValue, const KURL& url, StyleFetchedImage* image = 0)
    {
        return create(AtomicString(rawValue), url, image);
    }
    static GCRefPtr<CSSImageValue> create(const AtomicString& rawValue, const KURL& url, StyleFetchedImage* image = 0)
    {
        return BlinKit::GCWrapShared(new CSSImageValue(rawValue, url, image));
    }
    ~CSSImageValue();

    bool isCachePending() const { return m_isCachePending; }
    StyleFetchedImage* cachedImage(void) const;
    StyleFetchedImage* cacheImage(Document*);

    const String& url() const { return m_absoluteURL; }

#if 0 // BKTODO:
    void setReferrer(const Referrer& referrer) { m_referrer = referrer; }
    const Referrer& referrer() const { return m_referrer; }
#endif

    void reResolveURL(const Document&);

    String customCSSText() const;

    bool hasFailedOrCanceledSubresources() const;

    bool equals(const CSSImageValue&) const;

    bool knownToBeOpaque(const LayoutObject*) const;

    GCRefPtr<CSSImageValue> valueWithURLMadeAbsolute()
    {
        ASSERT(false); // BKTODO: return create(KURL(ParsedURLString, m_absoluteURL), m_cachedImage.get());
        return nullptr;
    }

    void setInitiator(const AtomicString& name) { m_initiatorName = name; }

    DECLARE_TRACE_AFTER_DISPATCH();
    void restoreCachedResourceIfNeeded(Document&) const;

private:
    CSSImageValue(const AtomicString& rawValue, const KURL&, StyleFetchedImage*);

    AtomicString m_relativeURL;
    AtomicString m_absoluteURL;
    // BKTODO: Referrer m_referrer;
    bool m_isCachePending;
    GCRefPtr<StyleFetchedImage> m_cachedImage;
    AtomicString m_initiatorName;
};

DEFINE_CSS_VALUE_TYPE_CASTS(CSSImageValue, isImageValue());

} // namespace blink

#endif // CSSImageValue_h
