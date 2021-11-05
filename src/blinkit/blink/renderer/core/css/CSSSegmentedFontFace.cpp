// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: CSSSegmentedFontFace.cpp
// Description: CSSSegmentedFontFace Class
//      Author: Ziming Li
//     Created: 2021-07-18
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2008 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "core/css/CSSSegmentedFontFace.h"

#include "core/css/CSSFontFace.h"
#include "core/css/CSSFontSelector.h"
#include "platform/RuntimeEnabledFeatures.h"
#include "platform/fonts/FontCache.h"
#include "platform/fonts/FontDescription.h"
#include "platform/fonts/FontFaceCreationParams.h"
#include "platform/fonts/SegmentedFontData.h"
#include "platform/fonts/SimpleFontData.h"

namespace blink {

CSSSegmentedFontFace::CSSSegmentedFontFace(CSSFontSelector* fontSelector, FontTraits traits)
    : m_fontSelector(fontSelector)
    , m_traits(traits)
    , m_firstNonCssConnectedFace(m_fontFaces.end())
{
}

CSSSegmentedFontFace::~CSSSegmentedFontFace()
{
    pruneTable();
#if !ENABLE(OILPAN)
    for (const auto& fontFace : m_fontFaces)
        fontFace->cssFontFace()->clearSegmentedFontFace();
#endif
}

void CSSSegmentedFontFace::pruneTable()
{
    // Make sure the glyph page tree prunes out all uses of this custom font.
    if (m_fontDataTable.isEmpty())
        return;

    m_fontDataTable.clear();
}

bool CSSSegmentedFontFace::isValid() const
{
    // Valid if at least one font face is valid.
    for (const auto& fontFace : m_fontFaces) {
        if (fontFace->cssFontFace()->isValid())
            return true;
    }
    return false;
}

void CSSSegmentedFontFace::fontFaceInvalidated()
{
    pruneTable();
}

void CSSSegmentedFontFace::addFontFace(PassRefPtrWillBeRawPtr<FontFace> prpFontFace, bool cssConnected)
{
    RefPtrWillBeRawPtr<FontFace> fontFace = prpFontFace;
    pruneTable();
    fontFace->cssFontFace()->setSegmentedFontFace(this);
    if (cssConnected) {
        m_fontFaces.insertBefore(m_firstNonCssConnectedFace, fontFace);
    } else {
        ASSERT(false); // BKTODO:
#if 0
        // This is the only place in Blink that is using addReturnIterator.
        FontFaceList::iterator iterator = m_fontFaces.addReturnIterator(fontFace);
        if (m_firstNonCssConnectedFace == m_fontFaces.end())
            m_firstNonCssConnectedFace = iterator;
#endif
    }
}

void CSSSegmentedFontFace::removeFontFace(PassRefPtrWillBeRawPtr<FontFace> prpFontFace)
{
    RefPtrWillBeRawPtr<FontFace> fontFace = prpFontFace;
    FontFaceList::iterator it = m_fontFaces.find(fontFace);
    if (it == m_fontFaces.end())
        return;

    if (it == m_firstNonCssConnectedFace)
        ++m_firstNonCssConnectedFace;
    m_fontFaces.erase(it);

    pruneTable();
    fontFace->cssFontFace()->clearSegmentedFontFace();
}

static void appendFontData(SegmentedFontData* newFontData, PassRefPtr<SimpleFontData> prpFaceFontData, const CSSFontFace::UnicodeRangeSet& ranges)
{
    RefPtr<SimpleFontData> faceFontData = prpFaceFontData;
    unsigned numRanges = ranges.size();
    if (!numRanges) {
        newFontData->appendRange(FontDataRange(0, 0x7FFFFFFF, faceFontData));
        return;
    }

    for (unsigned j = 0; j < numRanges; ++j)
        newFontData->appendRange(FontDataRange(ranges.rangeAt(j).from(), ranges.rangeAt(j).to(), faceFontData));
}

PassRefPtr<FontData> CSSSegmentedFontFace::getFontData(const FontDescription& fontDescription)
{
    if (!isValid())
        return nullptr;

    FontTraits desiredTraits = fontDescription.traits();
    FontCacheKey key = fontDescription.cacheKey(FontFaceCreationParams(), desiredTraits);

    RefPtr<SegmentedFontData>& fontData = m_fontDataTable.add(key.hash(), nullptr).storedValue->value;
    if (fontData && fontData->numRanges())
        return fontData; // No release, we have a reference to an object in the cache which should retain the ref count it has.

    if (!fontData)
        fontData = SegmentedFontData::create();

    FontDescription requestedFontDescription(fontDescription);
    requestedFontDescription.setTraits(m_traits);
    requestedFontDescription.setSyntheticBold(m_traits.weight() < FontWeight600 && desiredTraits.weight() >= FontWeight600);
    requestedFontDescription.setSyntheticItalic(m_traits.style() == FontStyleNormal && desiredTraits.style() == FontStyleItalic);

    for (FontFaceList::reverse_iterator it = m_fontFaces.rbegin(); it != m_fontFaces.rend(); ++it) {
        if (!(*it)->cssFontFace()->isValid())
            continue;
        if (RefPtr<SimpleFontData> faceFontData = (*it)->cssFontFace()->getFontData(requestedFontDescription)) {
            ASSERT(!faceFontData->isSegmented());
            appendFontData(fontData.get(), faceFontData.release(), (*it)->cssFontFace()->ranges());
        }
    }
    if (fontData->numRanges())
        return fontData; // No release, we have a reference to an object in the cache which should retain the ref count it has.

    return nullptr;
}

void CSSSegmentedFontFace::willUseFontData(const FontDescription& fontDescription, UChar32 character)
{
    for (FontFaceList::reverse_iterator it = m_fontFaces.rbegin(); it != m_fontFaces.rend(); ++it) {
        if ((*it)->loadStatus() != FontFace::Unloaded)
            break;
        if ((*it)->cssFontFace()->maybeScheduleFontLoad(fontDescription, character))
            break;
    }
}

void CSSSegmentedFontFace::willUseRange(const blink::FontDescription& fontDescription, const blink::FontDataRange& range)
{
    // Iterating backwards since later defined unicode-range faces override
    // previously defined ones, according to the CSS3 fonts module.
    // https://drafts.csswg.org/css-fonts/#composite-fonts
    for (FontFaceList::reverse_iterator it = m_fontFaces.rbegin(); it != m_fontFaces.rend(); ++it) {
        CSSFontFace* cssFontFace = (*it)->cssFontFace();
        if (cssFontFace->maybeScheduleFontLoad(fontDescription, range))
            break;
    }
}

bool CSSSegmentedFontFace::checkFont(const String& text) const
{
    for (const auto& fontFace : m_fontFaces) {
        if (fontFace->loadStatus() != FontFace::Loaded && fontFace->cssFontFace()->ranges().intersectsWith(text))
            return false;
    }
    return true;
}

void CSSSegmentedFontFace::match(const String& text, std::vector<GCRefPtr<FontFace>>& faces) const
{
    for (const auto& fontFace : m_fontFaces) {
        if (fontFace->cssFontFace()->ranges().intersectsWith(text))
            faces.emplace_back(fontFace);
    }
}

DEFINE_TRACE(CSSSegmentedFontFace)
{
#if ENABLE(OILPAN)
    visitor->trace(m_fontSelector);
    ASSERT(false); // BKTODO: visitor->trace(m_fontFaces);
#endif
}

}
