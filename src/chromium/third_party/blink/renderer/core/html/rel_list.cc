// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: rel_list.cc
// Description: RelList Class
//      Author: Ziming Li
//     Created: 2020-12-31
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/html/rel_list.h"

#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/dom/element.h"
#include "third_party/blink/renderer/core/html/html_element.h"
#include "third_party/blink/renderer/core/html_names.h"
// BKTODO: #include "third_party/blink/renderer/core/origin_trials/origin_trials.h"
#include "third_party/blink/renderer/platform/runtime_enabled_features.h"
#include "third_party/blink/renderer/platform/wtf/hash_map.h"

namespace blink {

RelList::RelList(Element* element)
    : DOMTokenList(*element, HTMLNames::relAttr) {}

static HashSet<AtomicString>& SupportedTokensLink() {
  DEFINE_STATIC_LOCAL(
      HashSet<AtomicString>, tokens,
      ({
          "preload", "preconnect", "dns-prefetch", "stylesheet", "import",
          "icon", "alternate", "prefetch", "prerender", "next", "manifest",
          "apple-touch-icon", "apple-touch-icon-precomposed", "canonical",
      }));

  return tokens;
}

static HashSet<AtomicString>& SupportedTokensAnchorAndArea() {
  DEFINE_STATIC_LOCAL(HashSet<AtomicString>, tokens,
                      ({
                          "noreferrer", "noopener",
                      }));

  return tokens;
}

bool RelList::ValidateTokenValue(const AtomicString& token_value,
                                 ExceptionState&) const {
  //  https://html.spec.whatwg.org/multipage/links.html#linkTypes
  if (GetElement().HasTagName(HTMLNames::linkTag)) {
    ASSERT(false); // BKTODO:
#if 0
    if (SupportedTokensLink().Contains(token_value) ||
        (RuntimeEnabledFeatures::ModulePreloadEnabled() &&
         token_value == "modulepreload")) {
      return true;
    }
#endif
  } else if ((GetElement().HasTagName(HTMLNames::aTag) ||
              GetElement().HasTagName(HTMLNames::areaTag)) &&
             SupportedTokensAnchorAndArea().Contains(token_value)) {
    return true;
  }
  return false;
}

}  // namespace blink
