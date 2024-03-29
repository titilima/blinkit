#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: ClientRectList.h
// Description: ClientRectList Class
//      Author: Ziming Li
//     Created: 2021-07-14
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2009 Apple Inc. All Rights Reserved.
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
 *
 */

#ifndef ClientRectList_h
#define ClientRectList_h

#include "blinkit/blink/renderer/core/dom/ClientRect.h"
#include "blinkit/blink/renderer/platform/geometry/FloatQuad.h"

namespace blink {

class ClientRectList final : public GarbageCollected<ClientRectList>, public ScriptWrappable
{
    DEFINE_WRAPPERTYPEINFO();
public:
    static ClientRectList* create()
    {
        return new ClientRectList;
    }
    static ClientRectList* create(const Vector<FloatQuad>& quads)
    {
        return new ClientRectList(quads);
    }

    template<typename Rects>
    static ClientRectList* create(const Rects& rects)
    {
        return new ClientRectList(rects);
    }

    unsigned length() const;
    ClientRect* item(unsigned index);
    ClientRect* anonymousIndexedGetter(unsigned index) { return item(index); }

    DECLARE_TRACE();

private:
    ClientRectList();

    template<typename Rects>
    explicit ClientRectList(const Rects& rects)
    {
        m_list.reserve(rects.size());
        for (const auto& r : rects)
            m_list.emplace_back(ClientRect::create(FloatRect(r)));
    }

    explicit ClientRectList(const Vector<FloatQuad>&);

    std::vector<Member<ClientRect>> m_list;
};

} // namespace blink

#endif // ClientRectList_h
