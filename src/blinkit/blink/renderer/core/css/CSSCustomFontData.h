// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: CSSCustomFontData.h
// Description: CSSCustomFontData Class
//      Author: Ziming Li
//     Created: 2021-07-29
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2013 Google, Inc.
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

#ifndef CSSCustomFontData_h
#define CSSCustomFontData_h

#include "core/css/CSSFontFaceSource.h"
#include "platform/fonts/CustomFontData.h"
#include "platform/heap/Handle.h"

namespace blink {

class CSSCustomFontData final : public CustomFontData {
public:
    enum FallbackVisibility { InvisibleFallback, VisibleFallback };

#if 0 // BKTODO:
    static PassRefPtr<CSSCustomFontData> create(RemoteFontFaceSource* source, FallbackVisibility visibility)
    {
        return adoptRef(new CSSCustomFontData(source, visibility));
    }
#endif

    ~CSSCustomFontData() override { }

    bool shouldSkipDrawing() const override
    {
        ASSERT(false); // BKTODO:
#if 0
        if (m_fontFaceSource)
            m_fontFaceSource->paintRequested();
#endif
        return m_fallbackVisibility == InvisibleFallback && m_isLoading;
    }

    void beginLoadIfNeeded() const override
    {
        ASSERT(false); // BKTODO:
#if 0
        if (!m_isLoading && m_fontFaceSource) {
            m_isLoading = true;
            m_fontFaceSource->beginLoadIfNeeded();
        }
#endif
    }

    bool isLoading() const override { return m_isLoading; }
    bool isLoadingFallback() const override { return true; }
    // BKTODO: void clearFontFaceSource() override { m_fontFaceSource = 0; }

private:
#if 0 // BKTODO:
    CSSCustomFontData(RemoteFontFaceSource* source, FallbackVisibility visibility)
        : m_fontFaceSource(source)
        , m_fallbackVisibility(visibility)
        , m_isLoading(false)
    {
        if (source)
            m_isLoading = source->isLoading();
    }
#endif

    // TODO(Oilpan): consider moving (Custom)FontFace hierarchy to the heap,
    // thereby making this reference a Member<>.
    // BKTODO: RawPtrWillBeWeakPersistent<RemoteFontFaceSource> m_fontFaceSource;
    FallbackVisibility m_fallbackVisibility;
    mutable bool m_isLoading;
};

} // namespace blink

#endif // CSSCustomFontData_h
