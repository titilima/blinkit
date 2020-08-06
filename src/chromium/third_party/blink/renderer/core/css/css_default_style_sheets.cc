// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: css_default_style_sheets.cc
// Description: CSSDefaultStyleSheets Class
//      Author: Ziming Li
//     Created: 2020-08-06
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 2004-2005 Allan Sandfeld Jensen (kde@carewolf.com)
 * Copyright (C) 2006, 2007 Nicholas Shanks (webkit@nickshanks.com)
 * Copyright (C) 2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012 Apple Inc. All
 * rights reserved.
 * Copyright (C) 2007 Alexey Proskuryakov <ap@webkit.org>
 * Copyright (C) 2007, 2008 Eric Seidel <eric@webkit.org>
 * Copyright (C) 2008, 2009 Torch Mobile Inc. All rights reserved.
 * (http://www.torchmobile.com/)
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

#include "third_party/blink/renderer/core/css/css_default_style_sheets.h"

#include "third_party/blink/renderer/core/css/media_query_evaluator.h"
#include "third_party/blink/renderer/core/css/rule_set.h"
#include "third_party/blink/renderer/core/css/style_sheet_contents.h"
#include "third_party/blink/renderer/core/css/style_sheet_contents.h"
#if 0 // BKTODO:
#include "third_party/blink/renderer/core/html/html_anchor_element.h"
#include "third_party/blink/renderer/core/html/html_html_element.h"
#else
#include "third_party/blink/renderer/core/html_names.h"
#endif
#include "third_party/blink/renderer/core/layout/layout_theme.h"
#include "third_party/blink/renderer/core/mathml_names.h"
#include "third_party/blink/renderer/platform/data_resource_helper.h"
#include "third_party/blink/renderer/platform/runtime_enabled_features.h"
#include "third_party/blink/renderer/platform/wtf/leak_annotations.h"

namespace blink {

using namespace html_names;

CSSDefaultStyleSheets& CSSDefaultStyleSheets::Instance() {
  static CSSDefaultStyleSheets css_default_style_sheets;
  return css_default_style_sheets;
}

static const MediaQueryEvaluator& ScreenEval() {
  static MediaQueryEvaluator static_screen_eval("screen");
  return static_screen_eval;
}

#if 0 // BKTODO:
static const MediaQueryEvaluator& PrintEval() {
  DEFINE_STATIC_LOCAL(Persistent<MediaQueryEvaluator>, static_print_eval,
                      (new MediaQueryEvaluator("print")));
  return *static_print_eval;
}
#endif

static std::shared_ptr<StyleSheetContents> ParseUASheet(const String& str) {
  // UA stylesheets always parse in the insecure context mode.
  std::unique_ptr<CSSParserContext> context = CSSParserContext::Create(kUASheetMode, SecureContextMode::kInsecureContext);
  std::shared_ptr<StyleSheetContents> sheet = StyleSheetContents::Create(context);
  sheet->ParseString(str);
  // User Agent stylesheets are parsed once for the lifetime of the renderer
  // process and are intentionally leaked.
  LEAK_SANITIZER_IGNORE_OBJECT(sheet);
  return sheet;
}

CSSDefaultStyleSheets::CSSDefaultStyleSheets()
    : media_controls_style_sheet_loader_(nullptr) {
  // Strict-mode rules.
  String default_rules = GetDataResourceAsASCIIString("html.css") +
                         LayoutTheme::GetTheme().ExtraDefaultStyleSheet();
  default_style_sheet_ = ParseUASheet(default_rules);

  // Quirks-mode rules.
  String quirks_rules = GetDataResourceAsASCIIString("quirks.css") +
                        LayoutTheme::GetTheme().ExtraQuirksStyleSheet();
  quirks_style_sheet_ = ParseUASheet(quirks_rules);

  InitializeDefaultStyles();

#if DCHECK_IS_ON()
  default_style_->CompactRulesIfNeeded();
  default_quirks_style_->CompactRulesIfNeeded();
  DCHECK(default_style_->UniversalRules()->IsEmpty());
  DCHECK(default_quirks_style_->UniversalRules()->IsEmpty());
#endif
}

void CSSDefaultStyleSheets::PrepareForLeakDetection() {
  // Clear the optional style sheets.
  ASSERT(false); // BKTODO:
#if 0
  media_controls_style_sheet_.Clear();
  mobile_viewport_style_sheet_.Clear();
  television_viewport_style_sheet_.Clear();
  xhtml_mobile_profile_style_sheet_.Clear();
  mathml_style_sheet_.Clear();
#endif
  // Initialize the styles that have the lazily loaded style sheets.
  InitializeDefaultStyles();
}

void CSSDefaultStyleSheets::InitializeDefaultStyles() {
  // This must be called only from constructor / PrepareForLeakDetection.
  default_style_ = RuleSet::Create();
  default_quirks_style_ = RuleSet::Create();

  default_style_->AddRulesFromSheet(DefaultStyleSheet(), ScreenEval());
  default_quirks_style_->AddRulesFromSheet(QuirksStyleSheet(), ScreenEval());
}

StyleSheetContents*
CSSDefaultStyleSheets::EnsureXHTMLMobileProfileStyleSheet() {
  if (!xhtml_mobile_profile_style_sheet_) {
    ASSERT(false); // BKTODO:
#if 0
    xhtml_mobile_profile_style_sheet_ =
        ParseUASheet(GetDataResourceAsASCIIString("xhtmlmp.css"));
#endif
  }
  return xhtml_mobile_profile_style_sheet_.get();
}

StyleSheetContents* CSSDefaultStyleSheets::EnsureMobileViewportStyleSheet() {
  if (!mobile_viewport_style_sheet_) {
    ASSERT(false); // BKTODO:
#if 0
    mobile_viewport_style_sheet_ =
        ParseUASheet(GetDataResourceAsASCIIString("viewportAndroid.css"));
#endif
  }
  return mobile_viewport_style_sheet_.get();
}

StyleSheetContents*
CSSDefaultStyleSheets::EnsureTelevisionViewportStyleSheet() {
  if (!television_viewport_style_sheet_) {
    ASSERT(false); // BKTODO:
#if 0
    television_viewport_style_sheet_ =
        ParseUASheet(GetDataResourceAsASCIIString("viewportTelevision.css"));
#endif
  }
  return television_viewport_style_sheet_.get();
}

bool CSSDefaultStyleSheets::EnsureDefaultStyleSheetsForElement(
    const Element& element) {
  bool changed_default_style = false;
  ASSERT(false); // BKTODO:
#if 0
  // FIXME: We should assert that the sheet only styles MathML elements.
  if (element.namespaceURI() == MathMLNames::mathmlNamespaceURI &&
      !mathml_style_sheet_) {
    mathml_style_sheet_ =
        ParseUASheet(GetDataResourceAsASCIIString("mathml.css"));
    default_style_->AddRulesFromSheet(MathmlStyleSheet(), ScreenEval());
    default_print_style_->AddRulesFromSheet(MathmlStyleSheet(), PrintEval());
    changed_default_style = true;
  }

  if (!media_controls_style_sheet_ && HasMediaControlsStyleSheetLoader() &&
      (IsHTMLVideoElement(element) || IsHTMLAudioElement(element))) {
    // FIXME: We should assert that this sheet only contains rules for <video>
    // and <audio>.
    media_controls_style_sheet_ =
        ParseUASheet(media_controls_style_sheet_loader_->GetUAStyleSheet());
    default_style_->AddRulesFromSheet(MediaControlsStyleSheet(), ScreenEval());
    default_print_style_->AddRulesFromSheet(MediaControlsStyleSheet(),
                                            PrintEval());
    changed_default_style = true;
  }
#endif

  DCHECK(!default_style_->Features().HasIdsInSelectors());
  return changed_default_style;
}

void CSSDefaultStyleSheets::SetMediaControlsStyleSheetLoader(
    std::unique_ptr<UAStyleSheetLoader> loader) {
  media_controls_style_sheet_loader_.swap(loader);
}

}  // namespace blink
