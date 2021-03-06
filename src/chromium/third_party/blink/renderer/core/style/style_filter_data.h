// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: style_filter_data.h
// Description: StyleFilterData Class
//      Author: Ziming Li
//     Created: 2020-08-03
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2011 Apple Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_STYLE_STYLE_FILTER_DATA_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_STYLE_STYLE_FILTER_DATA_H_

#include "third_party/blink/renderer/core/style/filter_operations.h"
#include "third_party/blink/renderer/platform/heap/handle.h"

namespace blink {

class StyleFilterData final : public GarbageCollectedFinalized<StyleFilterData> {
 public:
  BK_DECLARE_GC_NAME(StyleFilterData)

  static StyleFilterData* Create() { return new StyleFilterData; }

  StyleFilterData* Copy() const { return new StyleFilterData(*this); }

  bool operator==(const StyleFilterData&) const;
  bool operator!=(const StyleFilterData& o) const { return !(*this == o); }

  void Trace(blink::Visitor* visitor) { operations_.Trace(visitor); }

  FilterOperations operations_;

 private:
  StyleFilterData();
  explicit StyleFilterData(const StyleFilterData&);
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_STYLE_STYLE_FILTER_DATA_H_
