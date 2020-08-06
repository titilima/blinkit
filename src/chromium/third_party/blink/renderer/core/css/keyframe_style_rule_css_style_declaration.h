// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: keyframe_style_rule_css_style_declaration.h
// Description: KeyframeStyleRuleCSSStyleDeclaration Class
//      Author: Ziming Li
//     Created: 2020-08-06
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_CSS_KEYFRAME_STYLE_RULE_CSS_STYLE_DECLARATION_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_CSS_KEYFRAME_STYLE_RULE_CSS_STYLE_DECLARATION_H_

#include "third_party/blink/renderer/core/css/style_rule_css_style_declaration.h"

namespace blink {

class CSSKeyframeRule;

class KeyframeStyleRuleCSSStyleDeclaration final
    : public StyleRuleCSSStyleDeclaration {
 public:
  static std::shared_ptr<KeyframeStyleRuleCSSStyleDeclaration> Create(
      MutableCSSPropertyValueSet& property_set,
      CSSKeyframeRule* parent_rule) {
    return base::WrapShared(new KeyframeStyleRuleCSSStyleDeclaration(property_set, parent_rule));
  }

 private:
  KeyframeStyleRuleCSSStyleDeclaration(MutableCSSPropertyValueSet&,
                                       CSSKeyframeRule*);

  void DidMutate(MutationType) override;
  bool IsKeyframeStyle() const final { return true; }
};

}  // namespace blink

#endif
