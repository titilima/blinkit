// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: CSSDefaultStyleSheets.cpp
// Description: CSSDefaultStyleSheets Class
//      Author: Ziming Li
//     Created: 2021-07-29
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 2004-2005 Allan Sandfeld Jensen (kde@carewolf.com)
 * Copyright (C) 2006, 2007 Nicholas Shanks (webkit@nickshanks.com)
 * Copyright (C) 2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012 Apple Inc. All rights reserved.
 * Copyright (C) 2007 Alexey Proskuryakov <ap@webkit.org>
 * Copyright (C) 2007, 2008 Eric Seidel <eric@webkit.org>
 * Copyright (C) 2008, 2009 Torch Mobile Inc. All rights reserved. (http://www.torchmobile.com/)
 * Copyright (c) 2011, Code Aurora Forum. All rights reserved.
 * Copyright (C) Research In Motion Limited 2011. All rights reserved.
 * Copyright (C) 2012 Google Inc. All rights reserved.
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

#include "core/css/CSSDefaultStyleSheets.h"

#include "core/css/MediaQueryEvaluator.h"
#include "core/css/RuleSet.h"
#include "core/css/StyleSheetContents.h"
#include "core/html/HTMLAnchorElement.h"
#include "core/html/HTMLHtmlElement.h"
#include "core/layout/LayoutTheme.h"
#include "platform/PlatformResourceLoader.h"
#include "wtf/LeakAnnotations.h"

using namespace BlinKit;

namespace blink {

using namespace HTMLNames;

CSSDefaultStyleSheets& CSSDefaultStyleSheets::instance()
{
    static CSSDefaultStyleSheets *cssDefaultStyleSheets = GCWrapGlobal(new CSSDefaultStyleSheets);
    return *cssDefaultStyleSheets;
}

static const MediaQueryEvaluator& screenEval()
{
    static MediaQueryEvaluator *staticScreenEval = GCWrapGlobal(new MediaQueryEvaluator("screen"));
    return *staticScreenEval;
}

static const MediaQueryEvaluator& printEval()
{
    DEFINE_STATIC_LOCAL(OwnPtrWillBePersistent<MediaQueryEvaluator>, staticPrintEval, (adoptPtrWillBeNoop (new MediaQueryEvaluator("print"))));
    return *staticPrintEval;
}

static PassRefPtrWillBeRawPtr<StyleSheetContents> parseUASheet(const String& str)
{
    GCRefPtr<StyleSheetContents> sheet = StyleSheetContents::create(CSSParserContext(UASheetMode));
    sheet->parseString(str);
    return sheet.release();
}

CSSDefaultStyleSheets::CSSDefaultStyleSheets()
    : m_defaultStyle(nullptr)
    , m_defaultQuirksStyle(nullptr)
    , m_defaultStyleSheet(nullptr)
    , m_quirksStyleSheet(nullptr)
{
    m_defaultStyle = RuleSet::create();
    m_defaultQuirksStyle = RuleSet::create();

    // Strict-mode rules.
    String defaultRules = loadResourceAsASCIIString("html.css") + LayoutTheme::theme().extraDefaultStyleSheet();
    m_defaultStyleSheet = parseUASheet(defaultRules);
    m_defaultStyle->addRulesFromSheet(defaultStyleSheet(), screenEval());

    // Quirks-mode rules.
    String quirksRules = loadResourceAsASCIIString("quirks.css") + LayoutTheme::theme().extraQuirksStyleSheet();
    m_quirksStyleSheet = parseUASheet(quirksRules);
    m_defaultQuirksStyle->addRulesFromSheet(quirksStyleSheet(), screenEval());
}

void CSSDefaultStyleSheets::ensureDefaultStyleSheetsForElement(const Element& element, bool& changedDefaultStyle)
{
    ASSERT(!m_defaultStyle->features().hasIdsInSelectors());
    ASSERT(m_defaultStyle->features().siblingRules.isEmpty());
}

StyleSheetContents* CSSDefaultStyleSheets::defaultStyleSheet(void)
{
    return m_defaultStyleSheet.get();
}

StyleSheetContents* CSSDefaultStyleSheets::quirksStyleSheet(void)
{
    return m_quirksStyleSheet.get();
}

DEFINE_TRACE(CSSDefaultStyleSheets)
{
    visitor->trace(m_defaultStyle);
    visitor->trace(m_defaultQuirksStyle);
    visitor->trace(m_defaultStyleSheet);
    visitor->trace(m_quirksStyleSheet);
}

} // namespace blink
