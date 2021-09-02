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

// BKTODO: #include "core/MathMLNames.h"
#include "core/css/MediaQueryEvaluator.h"
#include "core/css/RuleSet.h"
#include "core/css/StyleSheetContents.h"
// BKTODO: #include "core/dom/Fullscreen.h"
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
    static MediaQueryEvaluator *staticScreenEval = GCMakeGlobal<MediaQueryEvaluator>("screen");
    return *staticScreenEval;
}

static const MediaQueryEvaluator& printEval()
{
    DEFINE_STATIC_LOCAL(OwnPtrWillBePersistent<MediaQueryEvaluator>, staticPrintEval, (adoptPtrWillBeNoop (new MediaQueryEvaluator("print"))));
    return *staticPrintEval;
}

static GCPassPtr<StyleSheetContents> parseUASheet(const String& str)
{
    GCMember<StyleSheetContents> sheet = StyleSheetContents::create(CSSParserContext(UASheetMode));
    sheet->parseString(str);
    return sheet.release();
}

CSSDefaultStyleSheets::CSSDefaultStyleSheets()
    : m_defaultStyle(nullptr)
    // BKTODO: , m_defaultMobileViewportStyle(nullptr)
    , m_defaultQuirksStyle(nullptr)
#if 0 // BKTODO:
    , m_defaultPrintStyle(nullptr)
    , m_defaultViewSourceStyle(nullptr)
    , m_defaultXHTMLMobileProfileStyle(nullptr)
#endif
    , m_defaultStyleSheet(nullptr)
    // BKTODO: , m_mobileViewportStyleSheet(nullptr)
    , m_quirksStyleSheet(nullptr)
#if 0 // BKTODO:
    , m_svgStyleSheet(nullptr)
    , m_mathmlStyleSheet(nullptr)
    , m_mediaControlsStyleSheet(nullptr)
    , m_fullscreenStyleSheet(nullptr)
#endif
{
    m_defaultStyle = RuleSet::create();
    // BKTODO: m_defaultPrintStyle = RuleSet::create();
    m_defaultQuirksStyle = RuleSet::create();

    // Strict-mode rules.
    String defaultRules = loadResourceAsASCIIString("html.css") + LayoutTheme::theme().extraDefaultStyleSheet();
    m_defaultStyleSheet = parseUASheet(defaultRules);
    m_defaultStyle->addRulesFromSheet(defaultStyleSheet(), screenEval());
    // BKTODO: m_defaultPrintStyle->addRulesFromSheet(defaultStyleSheet(), printEval());

    // Quirks-mode rules.
    String quirksRules = loadResourceAsASCIIString("quirks.css") + LayoutTheme::theme().extraQuirksStyleSheet();
    m_quirksStyleSheet = parseUASheet(quirksRules);
    m_defaultQuirksStyle->addRulesFromSheet(quirksStyleSheet(), screenEval());
}

#if 0 // BKTODO:
RuleSet* CSSDefaultStyleSheets::defaultViewSourceStyle()
{
    if (!m_defaultViewSourceStyle) {
        m_defaultViewSourceStyle = RuleSet::create();
        // Loaded stylesheet is leaked on purpose.
        RefPtrWillBeRawPtr<StyleSheetContents> stylesheet = parseUASheet(loadResourceAsASCIIString("view-source.css"));
        m_defaultViewSourceStyle->addRulesFromSheet(stylesheet.release().leakRef(), screenEval());
    }
    return m_defaultViewSourceStyle.get();
}

RuleSet* CSSDefaultStyleSheets::defaultXHTMLMobileProfileStyle()
{
    if (!m_defaultXHTMLMobileProfileStyle) {
        m_defaultXHTMLMobileProfileStyle = RuleSet::create();
        // Loaded stylesheet is leaked on purpose.
        RefPtrWillBeRawPtr<StyleSheetContents> stylesheet = parseUASheet(loadResourceAsASCIIString("xhtmlmp.css"));
        m_defaultXHTMLMobileProfileStyle->addRulesFromSheet(stylesheet.release().leakRef(), screenEval());
    }
    return m_defaultXHTMLMobileProfileStyle.get();
}

RuleSet* CSSDefaultStyleSheets::defaultMobileViewportStyle()
{
    if (!m_defaultMobileViewportStyle) {
        m_defaultMobileViewportStyle = RuleSet::create();
        m_mobileViewportStyleSheet = parseUASheet(loadResourceAsASCIIString("viewportAndroid.css"));
        m_defaultMobileViewportStyle->addRulesFromSheet(m_mobileViewportStyleSheet.get(), screenEval());
    }
    return m_defaultMobileViewportStyle.get();
}
#endif

void CSSDefaultStyleSheets::ensureDefaultStyleSheetsForElement(const Element& element, bool& changedDefaultStyle)
{
#if 0 // BKTODO:
    // FIXME: We should assert that the sheet only styles SVG elements.
    if (element.isSVGElement() && !m_svgStyleSheet) {
        m_svgStyleSheet = parseUASheet(loadResourceAsASCIIString("svg.css"));
        m_defaultStyle->addRulesFromSheet(svgStyleSheet(), screenEval());
        m_defaultPrintStyle->addRulesFromSheet(svgStyleSheet(), printEval());
        changedDefaultStyle = true;
    }

    // FIXME: We should assert that the sheet only styles MathML elements.
    if (element.namespaceURI() == MathMLNames::mathmlNamespaceURI
        && !m_mathmlStyleSheet) {
        m_mathmlStyleSheet = parseUASheet(loadResourceAsASCIIString("mathml.css"));
        m_defaultStyle->addRulesFromSheet(mathmlStyleSheet(), screenEval());
        m_defaultPrintStyle->addRulesFromSheet(mathmlStyleSheet(), printEval());
        changedDefaultStyle = true;
    }

    // FIXME: We should assert that this sheet only contains rules for <video> and <audio>.
    if (!m_mediaControlsStyleSheet && (isHTMLVideoElement(element) || isHTMLAudioElement(element))) {
        String mediaRules = loadResourceAsASCIIString(
            RuntimeEnabledFeatures::newMediaPlaybackUiEnabled() ?
            "mediaControlsNew.css" : "mediaControls.css") +
            LayoutTheme::theme().extraMediaControlsStyleSheet();
        m_mediaControlsStyleSheet = parseUASheet(mediaRules);
        m_defaultStyle->addRulesFromSheet(mediaControlsStyleSheet(), screenEval());
        m_defaultPrintStyle->addRulesFromSheet(mediaControlsStyleSheet(), printEval());
        changedDefaultStyle = true;
    }

    // FIXME: This only works because we Force recalc the entire document so the new sheet
    // is loaded for <html> and the correct styles apply to everyone.
    if (!m_fullscreenStyleSheet && Fullscreen::isFullScreen(element.document())) {
        String fullscreenRules = loadResourceAsASCIIString("fullscreen.css") + LayoutTheme::theme().extraFullScreenStyleSheet();
        m_fullscreenStyleSheet = parseUASheet(fullscreenRules);
        m_defaultStyle->addRulesFromSheet(fullscreenStyleSheet(), screenEval());
        m_defaultQuirksStyle->addRulesFromSheet(fullscreenStyleSheet(), screenEval());
        changedDefaultStyle = true;
    }
#endif

    ASSERT(!m_defaultStyle->features().hasIdsInSelectors());
    ASSERT(m_defaultStyle->features().siblingRules.isEmpty());
}

DEFINE_TRACE(CSSDefaultStyleSheets)
{
    visitor->trace(m_defaultStyle);
    // BKTODO: visitor->trace(m_defaultMobileViewportStyle);
    visitor->trace(m_defaultQuirksStyle);
#if 0 // BKTODO:
    visitor->trace(m_defaultPrintStyle);
    visitor->trace(m_defaultViewSourceStyle);
    visitor->trace(m_defaultXHTMLMobileProfileStyle);
#endif
    visitor->trace(m_defaultStyleSheet);
    // BKTODO: visitor->trace(m_mobileViewportStyleSheet);
    visitor->trace(m_quirksStyleSheet);
#if 0 // BKTODO:
    visitor->trace(m_svgStyleSheet);
    visitor->trace(m_mathmlStyleSheet);
    visitor->trace(m_mediaControlsStyleSheet);
    visitor->trace(m_fullscreenStyleSheet);
#endif
}

} // namespace blink
