/*
 * (C) 1999-2003 Lars Knoll (knoll@kde.org)
 * (C) 2002-2003 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2002, 2005, 2006, 2008, 2009, 2010, 2012 Apple Inc. All rights reserved.
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
 */

#include "core/css/CSSImportRule.h"

#include "core/css/CSSStyleSheet.h"
#include "core/css/MediaList.h"
#include "core/css/StyleRuleImport.h"
#include "core/css/StyleSheetContents.h"
#include "wtf/text/StringBuilder.h"

namespace blink {

CSSImportRule::CSSImportRule(StyleRuleImport* importRule, CSSStyleSheet* parent)
    : CSSRule(parent)
    , m_importRule(importRule)
{
}

CSSImportRule::~CSSImportRule()
{
#if !ENABLE(OILPAN)
    if (m_styleSheetCSSOMWrapper)
        m_styleSheetCSSOMWrapper->clearOwnerRule();

    if (m_mediaCSSOMWrapper)
        m_mediaCSSOMWrapper->clearParentRule();
#endif // ENABLE(OILPAN)
}

String CSSImportRule::href() const
{
    return m_importRule->href();
}

MediaList* CSSImportRule::media() const
{
    if (!m_mediaCSSOMWrapper)
        m_mediaCSSOMWrapper = MediaList::create(m_importRule->mediaQueries(), const_cast<CSSImportRule*>(this));
    return m_mediaCSSOMWrapper.get();
}

String CSSImportRule::cssText() const
{
    StringBuilder result;
    result.appendLiteral("@import url(\"");
    result.append(m_importRule->href());
    result.appendLiteral("\")");

    if (m_importRule->mediaQueries()) {
        String mediaText = m_importRule->mediaQueries()->mediaText();
        if (!mediaText.isEmpty()) {
            result.append(' ');
            result.append(mediaText);
        }
    }
    result.append(';');

    return result.toString();
}

CSSStyleSheet* CSSImportRule::styleSheet() const
{
    if (!m_importRule->styleSheet())
        return nullptr;

    if (!m_styleSheetCSSOMWrapper)
        m_styleSheetCSSOMWrapper = CSSStyleSheet::create(m_importRule->styleSheet(), const_cast<CSSImportRule*>(this));
    return m_styleSheetCSSOMWrapper.get();
}

void CSSImportRule::reattach(StyleRuleBase*)
{
    // FIXME: Implement when enabling caching for stylesheets with import rules.
    ASSERT_NOT_REACHED();
}

DEFINE_TRACE(CSSImportRule)
{
    visitor->trace(m_importRule);
    visitor->trace(m_mediaCSSOMWrapper);
    visitor->trace(m_styleSheetCSSOMWrapper);
    CSSRule::trace(visitor);
}

} // namespace blink
