#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: IntersectionObserverEntry.h
// Description: IntersectionObserverEntry Class
//      Author: Ziming Li
//     Created: 2022-01-23
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef IntersectionObserverEntry_h
#define IntersectionObserverEntry_h

#include "blinkit/blink/renderer/core/dom/ClientRect.h"
#include "blinkit/blink/renderer/platform/geometry/int_rect.h"
#include "blinkit/blink/renderer/platform/heap/Handle.h"

namespace blink {

class Element;

class IntersectionObserverEntry final : public GarbageCollectedFinalized<IntersectionObserverEntry>, public ScriptWrappable {
    DEFINE_WRAPPERTYPEINFO();
public:
    IntersectionObserverEntry(double timestamp, const IntRect& boundingClientRect, const IntRect& rootBounds, const IntRect& intersectionRect, Element*);
    ~IntersectionObserverEntry();

    double time() const { return m_time; }
    ClientRect* boundingClientRect() const { return m_boundingClientRect; }
    ClientRect* rootBounds() const { return m_rootBounds; }
    ClientRect* intersectionRect() const { return m_intersectionRect; }
    Element* target() const { return m_target.get(); }

    DECLARE_VIRTUAL_TRACE();

private:
    double m_time;
    Member<ClientRect> m_boundingClientRect;
    Member<ClientRect> m_rootBounds;
    Member<ClientRect> m_intersectionRect;
    RefPtrWillBeMember<Element> m_target;
};

} // namespace blink

#endif // IntersectionObserverEntry_h
