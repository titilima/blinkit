// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: css_namespace_rule.cc
// Description: CSSNamespaceRule Class
//      Author: Ziming Li
//     Created: 2020-10-11
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/css/css_namespace_rule.h"

#include "third_party/blink/renderer/core/css/css_markup.h"
#include "third_party/blink/renderer/core/css/style_rule_namespace.h"
#include "third_party/blink/renderer/platform/wtf/text/string_builder.h"

namespace blink {

CSSNamespaceRule::CSSNamespaceRule(StyleRuleNamespace* namespace_rule,
                                   CSSStyleSheet* parent)
    : CSSRule(parent), namespace_rule_(namespace_rule) {}

CSSNamespaceRule::~CSSNamespaceRule() = default;

String CSSNamespaceRule::cssText() const {
  StringBuilder result;
  result.Append("@namespace ");
  SerializeIdentifier(prefix(), result);
  if (!prefix().IsEmpty())
    result.Append(' ');
  result.Append("url(");
  result.Append(SerializeString(namespaceURI()));
  result.Append(");");
  return result.ToString();
}

AtomicString CSSNamespaceRule::namespaceURI() const {
  return namespace_rule_->Uri();
}

AtomicString CSSNamespaceRule::prefix() const {
  return namespace_rule_->Prefix();
}

}  // namespace blink
