// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: css_lazy_parsing_state.cc
// Description: CSSLazyParsingState Class
//      Author: Ziming Li
//     Created: 2020-09-20
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/css/parser/css_lazy_parsing_state.h"

#include "third_party/blink/renderer/core/css/parser/css_parser_token_stream.h"
#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/frame/use_counter.h"

namespace blink {

CSSLazyParsingState::CSSLazyParsingState(const CSSParserContext* context,
                                         const String& sheet_text,
                                         StyleSheetContents* contents)
    : context_(context),
      sheet_text_(sheet_text),
      owning_contents_(contents),
      should_use_count_(context_->IsUseCounterRecordingEnabled()) {}

const CSSParserContext* CSSLazyParsingState::Context() {
  DCHECK(owning_contents_);
  if (!should_use_count_) {
    DCHECK(!context_->IsUseCounterRecordingEnabled());
    return context_;
  }

  // Try as best as possible to grab a valid Document if the old Document has
  // gone away so we can still use UseCounter.
  if (!document_)
    document_ = owning_contents_->AnyOwnerDocument();

  if (!context_->IsDocumentHandleEqual(document_))
    ASSERT(false); // BKTODO: context_ = CSSParserContext::Create(context_, document_);
  return context_;
}

bool CSSLazyParsingState::ShouldLazilyParseProperties(
    const CSSSelectorList& selectors) const {
  //  Disallow lazy parsing for blocks which have before/after in their selector
  //  list. This ensures we don't cause a collectFeatures() when we trigger
  //  parsing for attr() functions which would trigger expensive invalidation
  //  propagation.
  for (const auto* s = selectors.FirstForCSSOM(); s;
       s = CSSSelectorList::Next(*s)) {
    for (const CSSSelector* current = s; current;
         current = current->TagHistory()) {
      const CSSSelector::PseudoType type(current->GetPseudoType());
      if (type == CSSSelector::kPseudoBefore ||
          type == CSSSelector::kPseudoAfter)
        return false;
      if (current->Relation() != CSSSelector::kSubSelector)
        break;
    }
  }
  return true;
}

void CSSLazyParsingState::Trace(blink::Visitor* visitor) {
  visitor->Trace(owning_contents_);
  visitor->Trace(document_);
  visitor->Trace(context_);
}

}  // namespace blink
