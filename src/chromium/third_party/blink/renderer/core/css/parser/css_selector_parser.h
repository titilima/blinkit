// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: css_selector_parser.h
// Description: CSSSelectorParser Class
//      Author: Ziming Li
//     Created: 2020-08-04
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_CSS_PARSER_CSS_SELECTOR_PARSER_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_CSS_PARSER_CSS_SELECTOR_PARSER_H_

#include <memory>
#include "base/macros.h"
#include "third_party/blink/renderer/core/core_export.h"
#include "third_party/blink/renderer/core/css/parser/css_parser_selector.h"
#include "third_party/blink/renderer/core/css/parser/css_parser_token_range.h"

namespace blink {

class CSSParserContext;
class CSSParserTokenStream;
class CSSParserObserver;
class CSSSelectorList;

// FIXME: We should consider building CSSSelectors directly instead of using
// the intermediate CSSParserSelector.
class CORE_EXPORT CSSSelectorParser {
  STACK_ALLOCATED();

 public:
  static CSSSelectorList ParseSelector(CSSParserTokenRange,
                                       std::unique_ptr<CSSParserContext> &);
  static CSSSelectorList ConsumeSelector(CSSParserTokenStream&,
                                         std::unique_ptr<CSSParserContext> &,
                                         CSSParserObserver*);

  static bool ConsumeANPlusB(CSSParserTokenRange&, std::pair<int, int>&);

 private:
  CSSSelectorParser(std::unique_ptr<CSSParserContext> &);

  // These will all consume trailing comments if successful

  CSSSelectorList ConsumeComplexSelectorList(CSSParserTokenRange&);
  CSSSelectorList ConsumeComplexSelectorList(CSSParserTokenStream&,
                                             CSSParserObserver*);
  CSSSelectorList ConsumeCompoundSelectorList(CSSParserTokenRange&);

  std::unique_ptr<CSSParserSelector> ConsumeComplexSelector(
      CSSParserTokenRange&);
  std::unique_ptr<CSSParserSelector> ConsumeCompoundSelector(
      CSSParserTokenRange&);
  // This doesn't include element names, since they're handled specially
  std::unique_ptr<CSSParserSelector> ConsumeSimpleSelector(
      CSSParserTokenRange&);

  bool ConsumeName(CSSParserTokenRange&,
                   AtomicString& name,
                   AtomicString& namespace_prefix);

  // These will return nullptr when the selector is invalid
  std::unique_ptr<CSSParserSelector> ConsumeId(CSSParserTokenRange&);
  std::unique_ptr<CSSParserSelector> ConsumeClass(CSSParserTokenRange&);
  std::unique_ptr<CSSParserSelector> ConsumePseudo(CSSParserTokenRange&);
  std::unique_ptr<CSSParserSelector> ConsumeAttribute(CSSParserTokenRange&);

  CSSSelector::RelationType ConsumeCombinator(CSSParserTokenRange&);
  CSSSelector::MatchType ConsumeAttributeMatch(CSSParserTokenRange&);
  CSSSelector::AttributeMatchType ConsumeAttributeFlags(CSSParserTokenRange&);

  const AtomicString& DefaultNamespace() const;
  const AtomicString& DetermineNamespace(const AtomicString& prefix);
  void PrependTypeSelectorIfNeeded(const AtomicString& namespace_prefix,
                                   bool has_element_name,
                                   const AtomicString& element_name,
                                   CSSParserSelector*);
  static std::unique_ptr<CSSParserSelector> AddSimpleSelectorToCompound(
      std::unique_ptr<CSSParserSelector> compound_selector,
      std::unique_ptr<CSSParserSelector> simple_selector);
  static std::unique_ptr<CSSParserSelector>
  SplitCompoundAtImplicitShadowCrossingCombinator(
      std::unique_ptr<CSSParserSelector> compound_selector);
  void RecordUsageAndDeprecations(const CSSSelectorList&);

  std::unique_ptr<const CSSParserContext> context_;

  bool failed_parsing_ = false;
  bool disallow_pseudo_elements_ = false;

  class DisallowPseudoElementsScope {
    STACK_ALLOCATED();

   public:
    DisallowPseudoElementsScope(CSSSelectorParser* parser)
        : parser_(parser), was_disallowed_(parser_->disallow_pseudo_elements_) {
      parser_->disallow_pseudo_elements_ = true;
    }

    ~DisallowPseudoElementsScope() {
      parser_->disallow_pseudo_elements_ = was_disallowed_;
    }

   private:
    CSSSelectorParser* parser_;
    bool was_disallowed_;
    DISALLOW_COPY_AND_ASSIGN(DisallowPseudoElementsScope);
  };
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_CSS_PARSER_CSS_SELECTOR_PARSER_H_
