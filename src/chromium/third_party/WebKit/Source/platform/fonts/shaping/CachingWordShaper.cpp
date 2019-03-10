// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: CachingWordShaper.cpp
// Description: CachingWordShaper Class
//      Author: Ziming Li
//     Created: 2019-03-08
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2015 Google Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY GOOGLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "platform/fonts/shaping/CachingWordShaper.h"

#include "platform/fonts/SimpleFontData.h"
#include "platform/fonts/shaping/CachingWordShapeIterator.h"
#include "platform/fonts/shaping/ShapeCache.h"
#if 0 // BKTODO:
#include "platform/fonts/shaping/ShapeResultBuffer.h"
#endif
#include "wtf/text/CharacterNames.h"

namespace blink {

float CachingWordShaper::width(const Font* font, const TextRun& run,
    HashSet<const SimpleFontData*>* fallbackFonts,
    FloatRect* glyphBounds)
{
    float width = 0;
    assert(false); // BKTODO:
#if 0
    RefPtr<ShapeResult> wordResult;
    CachingWordShapeIterator iterator(m_shapeCache, run, font);
    while (iterator.next(&wordResult)) {
        if (wordResult) {
            if (glyphBounds) {
                FloatRect adjustedBounds = wordResult->bounds();
                // Translate glyph bounds to the current glyph position which
                // is the total width before this glyph.
                adjustedBounds.setX(adjustedBounds.x() + width);
                glyphBounds->unite(adjustedBounds);
            }
            width += wordResult->width();
            if (fallbackFonts)
                wordResult->fallbackFonts(fallbackFonts);
        }
    }
#endif

    return width;
}

#if 0 // BKTODO:
static inline float shapeResultsForRun(ShapeCache* shapeCache, const Font* font,
    const TextRun& run, HashSet<const SimpleFontData*>* fallbackFonts,
    ShapeResultBuffer* resultsBuffer)
{
    CachingWordShapeIterator iterator(shapeCache, run, font);
    RefPtr<ShapeResult> wordResult;
    float totalWidth = 0;
    while (iterator.next(&wordResult)) {
        if (wordResult) {
            totalWidth += wordResult->width();
            if (fallbackFonts)
                wordResult->fallbackFonts(fallbackFonts);
            resultsBuffer->appendResult(wordResult.release());
        }
    }
    return totalWidth;
}
#endif

int CachingWordShaper::offsetForPosition(const Font* font, const TextRun& run, float targetX)
{
    assert(false); // BKTODO:
    return 0;
#if 0
    ShapeResultBuffer buffer;
    shapeResultsForRun(m_shapeCache, font, run, nullptr, &buffer);

    return buffer.offsetForPosition(run, targetX);
#endif
}

float CachingWordShaper::fillGlyphBuffer(const Font* font, const TextRun& run,
    HashSet<const SimpleFontData*>* fallbackFonts,
    GlyphBuffer* glyphBuffer, unsigned from, unsigned to)
{
    assert(false); // BKTODO:
    return 0.0;
#if 0
    ShapeResultBuffer buffer;
    shapeResultsForRun(m_shapeCache, font, run, fallbackFonts, &buffer);

    return buffer.fillGlyphBuffer(glyphBuffer, run, from, to);
#endif
}

float CachingWordShaper::fillGlyphBufferForTextEmphasis(const Font* font,
    const TextRun& run, const GlyphData* emphasisData, GlyphBuffer* glyphBuffer,
    unsigned from, unsigned to)
{
    assert(false); // BKTODO:
    return 0.0;
#if 0
    ShapeResultBuffer buffer;
    shapeResultsForRun(m_shapeCache, font, run, nullptr, &buffer);

    return buffer.fillGlyphBufferForTextEmphasis(glyphBuffer, run, emphasisData, from, to);
#endif
}

FloatRect CachingWordShaper::selectionRect(const Font* font, const TextRun& run,
    const FloatPoint& point, int height, unsigned from, unsigned to)
{
    assert(false); // BKTODO:
    return FloatRect();
#if 0
    ShapeResultBuffer buffer;
    float totalWidth = shapeResultsForRun(m_shapeCache, font, run, nullptr,
        &buffer);

    return buffer.selectionRect(run.direction(), totalWidth, point, height, from, to);
#endif
}

}; // namespace blink
