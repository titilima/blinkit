// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CSSNamespaceRule_h
#define CSSNamespaceRule_h

#include "core/css/CSSRule.h"

namespace blink {

class StyleRuleNamespace;

class CSSNamespaceRule final : public CSSRule {
    DEFINE_WRAPPERTYPEINFO();
public:
    static PassRefPtrWillBeRawPtr<CSSNamespaceRule> create(StyleRuleNamespace* rule, CSSStyleSheet* sheet)
    {
        return adoptRefWillBeNoop(new CSSNamespaceRule(rule, sheet));
    }

    ~CSSNamespaceRule() override;

    String cssText() const override;
    void reattach(StyleRuleBase*) override { }

    AtomicString namespaceURI() const;
    AtomicString prefix() const;

    DECLARE_VIRTUAL_TRACE();

private:
    CSSNamespaceRule(StyleRuleNamespace*, CSSStyleSheet*);

    CSSRule::Type type() const override { return NAMESPACE_RULE; }

    RefPtrWillBeMember<StyleRuleNamespace> m_namespaceRule;
};

DEFINE_CSS_RULE_TYPE_CASTS(CSSNamespaceRule, NAMESPACE_RULE);

} // namespace blink

#endif // CSSNamespaceRule_h
