// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: FontFaceCache.cpp
// Description: FontFaceCache Class
//      Author: Ziming Li
//     Created: 2021-07-19
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2007, 2008, 2011 Apple Inc. All rights reserved.
 * Copyright (C) 2013 Google Inc. All rights reserved.
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


#include "core/css/FontFaceCache.h"

#include "core/css/CSSFontSelector.h"
#include "core/css/CSSSegmentedFontFace.h"
#include "core/css/CSSValueList.h"
#include "core/css/FontFace.h"
#include "core/css/FontStyleMatcher.h"
#include "core/css/StyleRule.h"
#include "core/fetch/FontResource.h"
#include "core/fetch/ResourceFetcher.h"
#include "platform/FontFamilyNames.h"
#include "platform/fonts/FontDescription.h"
#include "wtf/text/AtomicString.h"


namespace blink {

FontFaceCache::FontFaceCache()
    : m_version(0)
{
}

void FontFaceCache::add(CSSFontSelector* cssFontSelector, const StyleRuleFontFace* fontFaceRule, FontFace *prpFontFace)
{
    if (zed::key_exists(m_styleRuleToFontFace, fontFaceRule))
        return;
    m_styleRuleToFontFace.emplace(fontFaceRule, prpFontFace);
    addFontFace(cssFontSelector, prpFontFace, true);
}

void FontFaceCache::addFontFace(CSSFontSelector* cssFontSelector, FontFace *prpFontFace, bool cssConnected)
{
    RefPtrWillBeRawPtr<FontFace> fontFace = prpFontFace;

    std::unique_ptr<TraitsMap> &traits = m_fontFaces[fontFace->family()];
    if (!traits)
        traits = std::make_unique<TraitsMap>();

    GCRefPtr<CSSSegmentedFontFace> &segmentedFontFace = (*traits)[fontFace->traits().bitfield()];
    if (!segmentedFontFace)
        segmentedFontFace = CSSSegmentedFontFace::create(cssFontSelector, fontFace->traits());

    segmentedFontFace->addFontFace(fontFace, cssConnected);
    if (cssConnected)
        m_cssConnectedFontFaces.insert(fontFace.get());

    m_fonts.erase(fontFace->family());
    ++m_version;
}

void FontFaceCache::remove(const StyleRuleFontFace* fontFaceRule)
{
    StyleRuleToFontFace::iterator it = m_styleRuleToFontFace.find(fontFaceRule);
    if (it != m_styleRuleToFontFace.end()) {
        removeFontFace(it->second.get(), true);
        m_styleRuleToFontFace.erase(it);
    }
}

void FontFaceCache::removeFontFace(FontFace* fontFace, bool cssConnected)
{
    FamilyToTraitsMap::iterator fontFacesIter = m_fontFaces.find(fontFace->family());
    if (fontFacesIter == m_fontFaces.end())
        return;
    TraitsMap* familyFontFaces = fontFacesIter->second.get();

    TraitsMap::iterator familyFontFacesIter = familyFontFaces->find(fontFace->traits().bitfield());
    if (familyFontFacesIter == familyFontFaces->end())
        return;
    GCRefPtr<CSSSegmentedFontFace> segmentedFontFace = familyFontFacesIter->second;

    segmentedFontFace->removeFontFace(fontFace);
    if (segmentedFontFace->isEmpty()) {
        familyFontFaces->erase(familyFontFacesIter);
        if (familyFontFaces->empty())
            m_fontFaces.erase(fontFacesIter);
    }
    m_fonts.erase(fontFace->family());
    if (cssConnected)
        m_cssConnectedFontFaces.erase(fontFace);

    ++m_version;
}

void FontFaceCache::clearCSSConnected()
{
    for (const auto& item : m_styleRuleToFontFace)
        removeFontFace(item.second.get(), true);
    m_styleRuleToFontFace.clear();
}

void FontFaceCache::clearAll()
{
    if (m_fontFaces.empty())
        return;

    m_fontFaces.clear();
    m_fonts.clear();
    m_styleRuleToFontFace.clear();
    m_cssConnectedFontFaces.clear();
    ++m_version;
}

CSSSegmentedFontFace* FontFaceCache::get(const FontDescription& fontDescription, const AtomicString& family)
{
    auto it = m_fontFaces.find(family);
    if (m_fontFaces.end() == it)
        return nullptr;

    TraitsMap* familyFontFaces = it->second.get();
    if (familyFontFaces->empty())
        return nullptr;

    std::unique_ptr<TraitsMap> &traitsMap = m_fonts[family];
    if (!traitsMap)
        traitsMap = std::make_unique<TraitsMap>();

    FontTraits traits = fontDescription.traits();
    GCRefPtr<CSSSegmentedFontFace> &face = (*traitsMap)[traits.bitfield()];
    if (!face) {
        for (const auto& item : *familyFontFaces) {
            CSSSegmentedFontFace* candidate = item.second.get();
            FontStyleMatcher styleMatcher(traits);
            if (!face || styleMatcher.isCandidateBetter(candidate, face.get()))
                face = candidate;
        }
    }
    return face.get();
}

DEFINE_TRACE(FontFaceCache)
{
#if ENABLE(OILPAN)
    visitor->trace(m_styleRuleToFontFace);
    visitor->trace(m_cssConnectedFontFaces);
#endif
}

}
