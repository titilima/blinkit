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
#include "platform/CrossOriginAttributeValue.h"
#include "platform/weborigin/Referrer.h"
#include "wtf/RefPtr.h"

namespace blink {

class Document;
class KURL;
class StyleFetchedImage;
class StyleImage;
class LayoutObject;

class CORE_EXPORT CSSImageValue : public CSSValue {
public:
    static PassRefPtrWillBeRawPtr<CSSImageValue> create(const KURL& url, StyleFetchedImage* image = 0)
    {
        return create(url.string(), url, image);
    }
    static PassRefPtrWillBeRawPtr<CSSImageValue> create(const String& rawValue, const KURL& url, StyleFetchedImage* image = 0)
    {
        return create(AtomicString(rawValue), url, image);
    }
    static PassRefPtrWillBeRawPtr<CSSImageValue> create(const AtomicString& rawValue, const KURL& url, StyleFetchedImage* image = 0)
    {
        return adoptRefWillBeNoop(new CSSImageValue(rawValue, url, image));
    }
    ~CSSImageValue();

    bool isCachePending() const { return m_isCachePending; }
    StyleFetchedImage* cachedImage() const { ASSERT(!isCachePending()); return m_cachedImage.get(); }
    StyleFetchedImage* cacheImage(Document*, CrossOriginAttributeValue = CrossOriginAttributeNotSet);

    const String& url() const { return m_absoluteURL; }

    void setReferrer(const Referrer& referrer) { m_referrer = referrer; }
    const Referrer& referrer() const { return m_referrer; }

    void reResolveURL(const Document&);

    String customCSSText() const;

    bool hasFailedOrCanceledSubresources() const;

    bool equals(const CSSImageValue&) const;

    bool knownToBeOpaque(const LayoutObject*) const;

    PassRefPtrWillBeRawPtr<CSSImageValue> valueWithURLMadeAbsolute()
    {
        return create(KURL(ParsedURLString, m_absoluteURL), m_cachedImage.get());
    }

    void setInitiator(const AtomicString& name) { m_initiatorName = name; }

    DECLARE_TRACE_AFTER_DISPATCH();
    void restoreCachedResourceIfNeeded(Document&) const;

private:
    CSSImageValue(const AtomicString& rawValue, const KURL&, StyleFetchedImage*);

    AtomicString m_relativeURL;
    AtomicString m_absoluteURL;
    Referrer m_referrer;
    bool m_isCachePending;
    RefPtrWillBeMember<StyleFetchedImage> m_cachedImage;
    AtomicString m_initiatorName;
};

DEFINE_CSS_VALUE_TYPE_CASTS(CSSImageValue, isImageValue());

} // namespace blink

#endif // CSSImageValue_h
