// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: MediaQuery.cpp
// Description: MediaQuery Class
//      Author: Ziming Li
//     Created: 2021-07-29
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * CSS Media Query
 *
 * Copyright (C) 2005, 2006 Kimmo Kinnunen <kimmo.t.kinnunen@nokia.com>.
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

#include "core/css/MediaQuery.h"

// BKTODO: #include "core/MediaTypeNames.h"
#include "core/css/MediaQueryExp.h"
#include "core/html/parser/HTMLParserIdioms.h"
#include "wtf/NonCopyingSort.h"
#include "wtf/text/StringBuilder.h"

namespace blink {

// http://dev.w3.org/csswg/cssom/#serialize-a-media-query
String MediaQuery::serialize() const
{
    StringBuilder result;
    switch (m_restrictor) {
    case MediaQuery::Only:
        result.appendLiteral("only ");
        break;
    case MediaQuery::Not:
        result.appendLiteral("not ");
        break;
    case MediaQuery::None:
        break;
    }

    if (m_expressions.empty()) {
        result.append(m_mediaType);
        return result.toString();
    }

    ASSERT(false); // BKTODO:
#if 0
    if (m_mediaType != MediaTypeNames::all || m_restrictor != None) {
        result.append(m_mediaType);
        result.appendLiteral(" and ");
    }
#endif

    result.append(m_expressions.at(0)->serialize());
    for (size_t i = 1; i < m_expressions.size(); ++i) {
        result.appendLiteral(" and ");
        result.append(m_expressions.at(i)->serialize());
    }
    return result.toString();
}

static bool expressionCompare(const OwnPtrWillBeMember<MediaQueryExp>& a, const OwnPtrWillBeMember<MediaQueryExp>& b)
{
    return codePointCompare(a->serialize(), b->serialize()) < 0;
}

PassOwnPtrWillBeRawPtr<MediaQuery> MediaQuery::createNotAll()
{
    ASSERT(false); // BKTODO: return adoptPtrWillBeNoop(new MediaQuery(MediaQuery::Not, MediaTypeNames::all, ExpressionHeapVector()));
    return nullptr;
}

PassOwnPtrWillBeRawPtr<MediaQuery> MediaQuery::create(Restrictor restrictor, String mediaType, ExpressionHeapVector &expressions)
{
    return adoptPtrWillBeNoop(new MediaQuery(restrictor, std::move(mediaType), expressions));
}

MediaQuery::MediaQuery(Restrictor r, String mediaType, ExpressionHeapVector &expressions)
    : m_restrictor(r)
    , m_mediaType(attemptStaticStringCreation(mediaType.lower()))
    , m_expressions(std::move(expressions))
{
    nonCopyingSort(m_expressions.begin(), m_expressions.end(), expressionCompare);

    // Remove all duplicated expressions.
    MediaQueryExp* key = 0;
    for (int i = m_expressions.size() - 1; i >= 0; --i) {
        MediaQueryExp* exp = m_expressions.at(i).get();

        if (key && *exp == *key)
            m_expressions.erase(m_expressions.begin() + i);
        else
            key = exp;
    }
}

MediaQuery::MediaQuery(const MediaQuery& o)
    : m_restrictor(o.m_restrictor)
    , m_mediaType(o.m_mediaType)
    , m_serializationCache(o.m_serializationCache)
{
    m_expressions.reserve(o.m_expressions.size());
    for (unsigned i = 0; i < o.m_expressions.size(); ++i)
        m_expressions.emplace_back(o.m_expressions[i]->copy());
}

MediaQuery::~MediaQuery()
{
}

// http://dev.w3.org/csswg/cssom/#compare-media-queries
bool MediaQuery::operator==(const MediaQuery& other) const
{
    return cssText() == other.cssText();
}

// http://dev.w3.org/csswg/cssom/#serialize-a-list-of-media-queries
String MediaQuery::cssText() const
{
    if (m_serializationCache.isNull())
        const_cast<MediaQuery*>(this)->m_serializationCache = serialize();

    return m_serializationCache;
}

DEFINE_TRACE(MediaQuery)
{
    // We don't support tracing of vectors of OwnPtrs (ie. OwnPtr<Vector<OwnPtr<MediaQuery>>>).
    // Since this is a transitional object we are just ifdef'ing it out when oilpan is not enabled.
#if ENABLE(OILPAN)
    visitor->trace(m_expressions);
#endif
}

}
