// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: MediaQuery.h
// Description: MediaQuery Class
//      Author: Ziming Li
//     Created: 2021-07-29
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * CSS Media Query
 *
 * Copyright (C) 2006 Kimmo Kinnunen <kimmo.t.kinnunen@nokia.com>.
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY
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

#ifndef MediaQuery_h
#define MediaQuery_h

#include "core/CoreExport.h"
#include "platform/heap/Handle.h"
#include "wtf/PassOwnPtr.h"
#include "wtf/Vector.h"
#include "wtf/text/StringHash.h"
#include "wtf/text/WTFString.h"
#include <utility>

namespace blink {
class MediaQueryExp;

using ExpressionHeapVector = std::vector<Member<MediaQueryExp>>;

class CORE_EXPORT MediaQuery {
    USING_FAST_MALLOC_WILL_BE_REMOVED(MediaQuery);
public:
    enum Restrictor {
        Only, Not, None
    };

    static std::unique_ptr<MediaQuery> create(Restrictor, String mediaType, ExpressionHeapVector &);
    static std::unique_ptr<MediaQuery> createNotAll();

    ~MediaQuery();

    Restrictor restrictor() const { return m_restrictor; }
    const ExpressionHeapVector& expressions() const { return m_expressions; }
    const String& mediaType() const { return m_mediaType; }
    bool operator==(const MediaQuery& other) const;
    String cssText() const;

    std::unique_ptr<MediaQuery> copy() const { return zed::wrap_unique(new MediaQuery(*this)); }

    DECLARE_TRACE();

private:
    MediaQuery(Restrictor, String mediaType, ExpressionHeapVector &);
    MediaQuery(const MediaQuery&);

    MediaQuery& operator=(const MediaQuery&) = delete;

    Restrictor m_restrictor;
    String m_mediaType;
    ExpressionHeapVector m_expressions;
    String m_serializationCache;

    String serialize() const;
};

} // namespace

#endif
