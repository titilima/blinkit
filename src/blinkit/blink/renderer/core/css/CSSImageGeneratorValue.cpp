// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: CSSImageGeneratorValue.cpp
// Description: CSSImageGeneratorValue Class
//      Author: Ziming Li
//     Created: 2021-07-18
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2008 Apple Inc.  All rights reserved.
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

#include "core/css/CSSImageGeneratorValue.h"

#include "core/css/CSSCrossfadeValue.h"
#include "core/css/CSSGradientValue.h"
#include "platform/graphics/Image.h"

using namespace BlinKit;

namespace blink {

CSSImageGeneratorValue::CSSImageGeneratorValue(ClassType classType)
    : CSSValue(classType)
{
}

CSSImageGeneratorValue::~CSSImageGeneratorValue()
{
}

void CSSImageGeneratorValue::addClient(const LayoutObject* layoutObject, const IntSize& size)
{
    ASSERT(layoutObject);
#if !ENABLE(OILPAN)
    ref();
#else
#if 0 // BKTODO:
    if (m_clients.isEmpty()) {
        ASSERT(!m_keepAlive);
        m_keepAlive = this;
    }
#else
    IncRef();
#endif
#endif

    if (!size.isEmpty())
        m_sizes.insert(size);

    LayoutObjectSizeCountMap::iterator it = m_clients.find(layoutObject);
    if (it == m_clients.end()) {
        m_clients.emplace(layoutObject, SizeAndCount(size, 1));
    } else {
        SizeAndCount& sizeCount = it->second;
        ++sizeCount.count;
    }
}

GCRefPtr<CSSImageGeneratorValue> CSSImageGeneratorValue::valueWithURLsMadeAbsolute()
{
    if (isCrossfadeValue())
        return toCSSCrossfadeValue(this)->valueWithURLsMadeAbsolute();
    return GCWrapShared(this);
}

void CSSImageGeneratorValue::removeClient(const LayoutObject* layoutObject)
{
    ASSERT(layoutObject);
    LayoutObjectSizeCountMap::iterator it = m_clients.find(layoutObject);
    ASSERT_WITH_SECURITY_IMPLICATION(it != m_clients.end());

    IntSize removedImageSize;
    SizeAndCount& sizeCount = it->second;
    IntSize size = sizeCount.size;
    if (!size.isEmpty()) {
        m_sizes.erase(size);
        if (!m_sizes.contains(size))
            m_images.remove(size);
    }

    if (!--sizeCount.count)
        m_clients.erase(layoutObject);

#if !ENABLE(OILPAN)
    deref();
#else
#if 0 // BKTODO:
    if (m_clients.isEmpty()) {
        ASSERT(m_keepAlive);
        m_keepAlive.clear();
    }
#else
    Release();
#endif
#endif
}

Image* CSSImageGeneratorValue::getImage(const LayoutObject* layoutObject, const IntSize& size)
{
    LayoutObjectSizeCountMap::iterator it = m_clients.find(layoutObject);
    if (it != m_clients.end()) {
        SizeAndCount& sizeCount = it->second;
        IntSize oldSize = sizeCount.size;
        if (oldSize != size) {
            GCGuard _(*this);
            removeClient(layoutObject);
            addClient(layoutObject, size);
        }
    }

    // Don't generate an image for empty sizes.
    if (size.isEmpty())
        return nullptr;

    // Look up the image in our cache.
    return m_images.get(size);
}

void CSSImageGeneratorValue::putImage(const IntSize& size, PassRefPtr<Image> image)
{
    m_images.add(size, image);
}

PassRefPtr<Image> CSSImageGeneratorValue::image(const LayoutObject* layoutObject, const IntSize& size)
{
    switch (classType()) {
    case CrossfadeClass:
        return toCSSCrossfadeValue(this)->image(layoutObject, size);
    case LinearGradientClass:
        return toCSSLinearGradientValue(this)->image(layoutObject, size);
    case RadialGradientClass:
        return toCSSRadialGradientValue(this)->image(layoutObject, size);
    default:
        ASSERT_NOT_REACHED();
    }
    return nullptr;
}

bool CSSImageGeneratorValue::isFixedSize() const
{
    switch (classType()) {
    case CrossfadeClass:
        return toCSSCrossfadeValue(this)->isFixedSize();
    case LinearGradientClass:
        return toCSSLinearGradientValue(this)->isFixedSize();
    case RadialGradientClass:
        return toCSSRadialGradientValue(this)->isFixedSize();
    default:
        ASSERT_NOT_REACHED();
    }
    return false;
}

IntSize CSSImageGeneratorValue::fixedSize(const LayoutObject* layoutObject)
{
    switch (classType()) {
    case CrossfadeClass:
        return toCSSCrossfadeValue(this)->fixedSize(layoutObject);
    case LinearGradientClass:
        return toCSSLinearGradientValue(this)->fixedSize(layoutObject);
    case RadialGradientClass:
        return toCSSRadialGradientValue(this)->fixedSize(layoutObject);
    default:
        ASSERT_NOT_REACHED();
    }
    return IntSize();
}

bool CSSImageGeneratorValue::isPending() const
{
    switch (classType()) {
    case CrossfadeClass:
        return toCSSCrossfadeValue(this)->isPending();
    case LinearGradientClass:
        return toCSSLinearGradientValue(this)->isPending();
    case RadialGradientClass:
        return toCSSRadialGradientValue(this)->isPending();
    default:
        ASSERT_NOT_REACHED();
    }
    return false;
}

bool CSSImageGeneratorValue::knownToBeOpaque(const LayoutObject* layoutObject) const
{
    switch (classType()) {
    case CrossfadeClass:
        return toCSSCrossfadeValue(this)->knownToBeOpaque(layoutObject);
    case LinearGradientClass:
        return toCSSLinearGradientValue(this)->knownToBeOpaque(layoutObject);
    case RadialGradientClass:
        return toCSSRadialGradientValue(this)->knownToBeOpaque(layoutObject);
    default:
        ASSERT_NOT_REACHED();
    }
    return false;
}

void CSSImageGeneratorValue::loadSubimages(Document* document)
{
    switch (classType()) {
    case CrossfadeClass:
        toCSSCrossfadeValue(this)->loadSubimages(document);
        break;
    case LinearGradientClass:
        toCSSLinearGradientValue(this)->loadSubimages(document);
        break;
    case RadialGradientClass:
        toCSSRadialGradientValue(this)->loadSubimages(document);
        break;
    default:
        ASSERT_NOT_REACHED();
    }
}

} // namespace blink
