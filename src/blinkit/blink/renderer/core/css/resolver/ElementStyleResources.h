// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: ElementStyleResources.h
// Description: ElementStyleResources Class
//      Author: Ziming Li
//     Created: 2021-07-13
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 * Copyright (C) 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011 Apple Inc. All rights reserved.
 * Copyright (C) 2013 Google Inc. All rights reserved.
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
 *
 */

#ifndef ElementStyleResources_h
#define ElementStyleResources_h

#include <unordered_set>
#include "core/CSSPropertyNames.h"
// BKTODO: #include "platform/CrossOriginAttributeValue.h"
#include "platform/graphics/Color.h"
#include "platform/heap/Handle.h"
#include "wtf/HashMap.h"
#include "wtf/Noncopyable.h"

namespace blink {

class CSSCursorImageValue;
class CSSImageGeneratorValue;
class CSSImageSetValue;
class CSSImageValue;
class CSSSVGDocumentValue;
class CSSValue;
class ComputedStyle;
class FilterOperation;
class StyleImage;
class StylePendingImage;

// Holds information about resources, requested by stylesheets.
// Lifetime: per-element style resolve.
class ElementStyleResources {
    STACK_ALLOCATED();
    WTF_MAKE_NONCOPYABLE(ElementStyleResources);
public:
    ElementStyleResources(Document&, float deviceScaleFactor);

    PassRefPtrWillBeRawPtr<StyleImage> styleImage(CSSPropertyID, const CSSValue&);
    PassRefPtrWillBeRawPtr<StyleImage> cachedOrPendingFromValue(CSSPropertyID, const CSSImageValue&);
    PassRefPtrWillBeRawPtr<StyleImage> setOrPendingFromValue(CSSPropertyID, const CSSImageSetValue&);

    void loadPendingResources(ComputedStyle*);

    // BKTODO: void addPendingSVGDocument(FilterOperation*, CSSSVGDocumentValue*);

private:
    PassRefPtrWillBeRawPtr<StyleImage> cursorOrPendingFromValue(CSSPropertyID, const CSSCursorImageValue&);
    PassRefPtrWillBeRawPtr<StyleImage> generatedOrPendingFromValue(CSSPropertyID, const CSSImageGeneratorValue&);

    // BKTODO: void loadPendingSVGDocuments(ComputedStyle*);
    void loadPendingImages(ComputedStyle*);

    // BKTODO: PassRefPtrWillBeRawPtr<StyleImage> loadPendingImage(StylePendingImage*, CrossOriginAttributeValue = CrossOriginAttributeNotSet);

    RawPtrWillBeMember<Document> m_document;
    std::unordered_set<CSSPropertyID> m_pendingImageProperties;
    // BKTODO: WillBeHeapHashMap<RawPtrWillBeMember<FilterOperation>, RefPtrWillBeMember<CSSSVGDocumentValue>> m_pendingSVGDocuments;
    float m_deviceScaleFactor;
};

} // namespace blink

#endif // ElementStyleResources_h
