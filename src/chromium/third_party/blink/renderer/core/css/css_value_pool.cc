// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: css_value_pool.cc
// Description: CSSValuePool Class
//      Author: Ziming Li
//     Created: 2020-08-27
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2011, 2012 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "third_party/blink/renderer/core/css/css_value_pool.h"

namespace blink {

using namespace cssvalue;

CSSValuePool& CssValuePool() {
  static CSSValuePool s_pool;
  return s_pool;
}

CSSValuePool::CSSValuePool()
    : inherited_value_(new CSSInheritedValue),
      initial_value_(new CSSInitialValue()),
      unset_value_(new CSSUnsetValue),
      color_transparent_(new CSSColorValue(Color::kTransparent)),
      color_white_(new CSSColorValue(Color::kWhite)),
      color_black_(new CSSColorValue(Color::kBlack)) {
  identifier_value_cache_.resize(numCSSValueKeywords);
  pixel_value_cache_.resize(kMaximumCacheableIntegerValue + 1);
  percent_value_cache_.resize(kMaximumCacheableIntegerValue + 1);
  number_value_cache_.resize(kMaximumCacheableIntegerValue + 1);
}

}  // namespace blink
