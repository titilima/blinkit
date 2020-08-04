// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: css_rule_list.h
// Description: CSSRuleList Class
//      Author: Ziming Li
//     Created: 2020-08-04
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

/*
 * (C) 1999-2003 Lars Knoll (knoll@kde.org)
 * (C) 2002-2003 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2002, 2006, 2012 Apple Computer, Inc.
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

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_CSS_CSS_RULE_LIST_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_CSS_CSS_RULE_LIST_H_

#include "base/macros.h"
#include "base/memory/ptr_util.h"
#include "base/memory/scoped_refptr.h"
#include "third_party/blink/renderer/platform/bindings/script_wrappable.h"
#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/wtf/vector.h"

namespace blink {

class CSSRule;
class CSSStyleSheet;

class CSSRuleList : public ScriptWrappable {
  DEFINE_WRAPPERTYPEINFO();

 public:
  virtual unsigned length() const = 0;
  virtual CSSRule* item(unsigned index) const = 0;

  virtual CSSStyleSheet* GetStyleSheet() const = 0;

 protected:
  CSSRuleList() = default;

 private:
  GCType GetGCType(void) const final { return GC_MANUAL; }

  DISALLOW_COPY_AND_ASSIGN(CSSRuleList);
};

class StaticCSSRuleList final : public CSSRuleList {
 public:
  static std::unique_ptr<StaticCSSRuleList> Create() { return base::WrapUnique(new StaticCSSRuleList); }

  HeapVector<Member<CSSRule>>& Rules() { return rules_; }

  CSSStyleSheet* GetStyleSheet() const override { return nullptr; }

 private:
  StaticCSSRuleList();

  unsigned length() const override { return rules_.size(); }
  CSSRule* item(unsigned index) const override {
    return index < rules_.size() ? rules_[index].Get() : nullptr;
  }

  HeapVector<Member<CSSRule>> rules_;
};

template <class Rule>
class LiveCSSRuleList final : public CSSRuleList {
 public:
  static std::unique_ptr<LiveCSSRuleList<Rule>> Create(Rule* rule) {
    return base::WrapUnique(new LiveCSSRuleList(rule));
  }

 private:
  LiveCSSRuleList(Rule* rule) : rule_(rule) {}

  unsigned length() const override { return rule_->length(); }
  CSSRule* item(unsigned index) const override { return rule_->Item(index); }
  CSSStyleSheet* GetStyleSheet() const override {
    return rule_->parentStyleSheet();
  }

  Member<Rule> rule_;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_CSS_CSS_RULE_LIST_H_
