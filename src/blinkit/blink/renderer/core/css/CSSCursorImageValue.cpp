/*
 * Copyright (C) 2006 Rob Buis <buis@kde.org>
 *           (C) 2008 Nikolas Zimmermann <zimmermann@kde.org>
 * Copyright (C) 2008 Apple Inc. All rights reserved.
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

#include "core/css/CSSCursorImageValue.h"

#include "core/SVGNames.h"
#include "core/css/CSSImageSetValue.h"
#include "core/fetch/ImageResource.h"
#include "core/style/StyleFetchedImage.h"
#include "core/style/StyleFetchedImageSet.h"
#include "core/style/StyleImage.h"
#include "core/svg/SVGCursorElement.h"
#include "core/svg/SVGLengthContext.h"
#include "core/svg/SVGURIReference.h"
#include "wtf/MathExtras.h"
#include "wtf/text/StringBuilder.h"
#include "wtf/text/WTFString.h"

namespace blink {

static inline SVGCursorElement* resourceReferencedByCursorElement(const String& url, TreeScope& treeScope)
{
    Element* element = SVGURIReference::targetElementFromIRIString(url, treeScope);
    return isSVGCursorElement(element) ? toSVGCursorElement(element) : nullptr;
}

CSSCursorImageValue::CSSCursorImageValue(PassRefPtrWillBeRawPtr<CSSValue> imageValue, bool hotSpotSpecified, const IntPoint& hotSpot)
    : CSSValue(CursorImageClass)
    , m_imageValue(imageValue)
    , m_hotSpotSpecified(hotSpotSpecified)
    , m_hotSpot(hotSpot)
    , m_isCachePending(true)
{
}

CSSCursorImageValue::~CSSCursorImageValue()
{
    // The below teardown is all handled by weak pointer processing in oilpan.
#if !ENABLE(OILPAN)
    if (!isSVGCursor())
        return;

    String url = toCSSImageValue(m_imageValue.get())->url();

    for (SVGElement* referencedElement : m_referencedElements) {
        referencedElement->cursorImageValueRemoved();
        if (SVGCursorElement* cursorElement = resourceReferencedByCursorElement(url, referencedElement->treeScope()))
            cursorElement->removeClient(referencedElement);
    }
#endif
}

String CSSCursorImageValue::customCSSText() const
{
    StringBuilder result;
    result.append(m_imageValue->cssText());
    if (m_hotSpotSpecified) {
        result.append(' ');
        result.appendNumber(m_hotSpot.x());
        result.append(' ');
        result.appendNumber(m_hotSpot.y());
    }
    return result.toString();
}

bool CSSCursorImageValue::updateIfSVGCursorIsUsed(Element* element)
{
    if (!element || !element->isSVGElement())
        return false;

    if (!isSVGCursor())
        return false;

    String url = toCSSImageValue(m_imageValue.get())->url();
    if (SVGCursorElement* cursorElement = resourceReferencedByCursorElement(url, element->treeScope())) {
        // FIXME: This will override hot spot specified in CSS, which is probably incorrect.
        SVGLengthContext lengthContext(0);
        m_hotSpotSpecified = true;
        float x = roundf(cursorElement->x()->currentValue()->value(lengthContext));
        m_hotSpot.setX(static_cast<int>(x));

        float y = roundf(cursorElement->y()->currentValue()->value(lengthContext));
        m_hotSpot.setY(static_cast<int>(y));

        if (cachedImageURL() != element->document().completeURL(cursorElement->href()->currentValue()->value()))
            clearImageResource();

        SVGElement* svgElement = toSVGElement(element);
#if !ENABLE(OILPAN)
        m_referencedElements.add(svgElement);
#endif
        svgElement->setCursorImageValue(this);
        cursorElement->addClient(svgElement);
        return true;
    }

    return false;
}

bool CSSCursorImageValue::isCachePending(float deviceScaleFactor) const
{
    // Need to delegate completely so that changes in device scale factor can be handled appropriately.
    if (m_imageValue->isImageSetValue())
        return toCSSImageSetValue(*m_imageValue).isCachePending(deviceScaleFactor);
    return m_isCachePending;
}

StyleImage* CSSCursorImageValue::cachedImage(float deviceScaleFactor) const
{
    ASSERT(!isCachePending(deviceScaleFactor));

    if (m_imageValue->isImageSetValue())
        return toCSSImageSetValue(*m_imageValue).cachedImageSet(deviceScaleFactor);
    return m_cachedImage.get();
}

StyleImage* CSSCursorImageValue::cacheImage(Document* document, float deviceScaleFactor)
{
    if (m_imageValue->isImageSetValue())
        return toCSSImageSetValue(*m_imageValue).cacheImageSet(document, deviceScaleFactor);

    if (m_isCachePending) {
        m_isCachePending = false;

        // For SVG images we need to lazily substitute in the correct URL. Rather than attempt
        // to change the URL of the CSSImageValue (which would then change behavior like cssText),
        // we create an alternate CSSImageValue to use.
        if (isSVGCursor() && document) {
            RefPtrWillBeRawPtr<CSSImageValue> imageValue = toCSSImageValue(m_imageValue.get());
            // FIXME: This will fail if the <cursor> element is in a shadow DOM (bug 59827)
            if (SVGCursorElement* cursorElement = resourceReferencedByCursorElement(imageValue->url(), *document)) {
                RefPtrWillBeRawPtr<CSSImageValue> svgImageValue = CSSImageValue::create(document->completeURL(cursorElement->href()->currentValue()->value()));
                svgImageValue->setReferrer(imageValue->referrer());
                m_cachedImage = svgImageValue->cacheImage(document);
                return m_cachedImage.get();
            }
        }

        if (m_imageValue->isImageValue())
            m_cachedImage = toCSSImageValue(*m_imageValue).cacheImage(document);
    }

    if (m_cachedImage && m_cachedImage->isImageResource())
        return toStyleFetchedImage(m_cachedImage);
    return nullptr;
}

bool CSSCursorImageValue::isSVGCursor() const
{
    if (m_imageValue->isImageValue()) {
        RefPtrWillBeRawPtr<CSSImageValue> imageValue = toCSSImageValue(m_imageValue.get());
        KURL kurl(ParsedURLString, imageValue->url());
        return kurl.hasFragmentIdentifier();
    }
    return false;
}

String CSSCursorImageValue::cachedImageURL()
{
    if (!m_cachedImage || !m_cachedImage->isImageResource())
        return String();
    return toStyleFetchedImage(m_cachedImage)->cachedImage()->url().string();
}

void CSSCursorImageValue::clearImageResource()
{
    m_cachedImage = nullptr;
    m_isCachePending = true;
}

#if !ENABLE(OILPAN)
void CSSCursorImageValue::removeReferencedElement(SVGElement* element)
{
    m_referencedElements.remove(element);
}
#endif

bool CSSCursorImageValue::equals(const CSSCursorImageValue& other) const
{
    return (m_hotSpotSpecified ? other.m_hotSpotSpecified && m_hotSpot == other.m_hotSpot : !other.m_hotSpotSpecified)
        && compareCSSValuePtr(m_imageValue, other.m_imageValue);
}

DEFINE_TRACE_AFTER_DISPATCH(CSSCursorImageValue)
{
    visitor->trace(m_imageValue);
    visitor->trace(m_cachedImage);
    CSSValue::traceAfterDispatch(visitor);
}

} // namespace blink
