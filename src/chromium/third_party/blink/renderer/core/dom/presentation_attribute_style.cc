// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: presentation_attribute_style.cc
// Description: ComputePresentationAttributeStyle
//      Author: Ziming Li
//     Created: 2021-01-11
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2013 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "third_party/blink/renderer/core/dom/presentation_attribute_style.h"

#include <algorithm>

#include "base/macros.h"
#include "blinkit/gc/gc_static.h"
#include "third_party/blink/public/platform/platform.h"
#include "third_party/blink/renderer/core/css/css_property_value_set.h"
#include "third_party/blink/renderer/core/dom/attribute.h"
#include "third_party/blink/renderer/core/dom/element.h"
#include "third_party/blink/renderer/core/html/forms/html_input_element.h"
#include "third_party/blink/renderer/platform/scheduler/public/thread.h"
// BKTODO: #include "third_party/blink/renderer/platform/scheduler/public/thread_scheduler.h"
#include "third_party/blink/renderer/platform/wtf/hash_functions.h"
#include "third_party/blink/renderer/platform/wtf/hash_map.h"
#include "third_party/blink/renderer/platform/wtf/text/cstring.h"

using namespace BlinKit;

namespace blink {

using namespace HTMLNames;

struct PresentationAttributeCacheKey {
  PresentationAttributeCacheKey() : tag_name(nullptr) {}
  StringImpl* tag_name;
  Vector<std::pair<StringImpl*, AtomicString>, 3> attributes_and_values;
};

static bool operator!=(const PresentationAttributeCacheKey& a,
                       const PresentationAttributeCacheKey& b) {
  if (a.tag_name != b.tag_name)
    return true;
  return a.attributes_and_values != b.attributes_and_values;
}

struct PresentationAttributeCacheEntry final
    : public GarbageCollectedFinalized<PresentationAttributeCacheEntry> {
 public:
  void Trace(blink::Visitor* visitor) { visitor->Trace(value); }

  PresentationAttributeCacheKey key;
  Member<CSSPropertyValueSet> value;
};

using PresentationAttributeCache =
    HeapHashMap<unsigned,
                Member<PresentationAttributeCacheEntry>>;
static PresentationAttributeCache& GetPresentationAttributeCache() {
  static GCStaticWrapper<PresentationAttributeCache, TracePolicy<PresentationAttributeCache>> cache;
  return cache.GetAsReference();
}

static bool AttributeNameSort(const std::pair<StringImpl*, AtomicString>& p1,
                              const std::pair<StringImpl*, AtomicString>& p2) {
  // Sort based on the attribute name pointers. It doesn't matter what the order
  // is as long as it is always the same.
  return p1.first < p2.first;
}

static void MakePresentationAttributeCacheKey(
    Element& element,
    PresentationAttributeCacheKey& result) {
  // FIXME: Enable for SVG.
  if (!element.IsHTMLElement())
    return;
  // Interpretation of the size attributes on <input> depends on the type
  // attribute.
  if (IsHTMLInputElement(element))
    return;
  AttributeCollection attributes = element.AttributesWithoutUpdate();
  for (const Attribute& attr : attributes) {
    if (!element.IsPresentationAttribute(attr.GetName()))
      continue;
    if (!attr.NamespaceURI().IsNull())
      return;
    // FIXME: Background URL may depend on the base URL and can't be shared.
    // Disallow caching.
    if (attr.GetName() == backgroundAttr)
      return;
    result.attributes_and_values.push_back(
        std::make_pair(attr.LocalName().Impl(), attr.Value()));
  }
  if (result.attributes_and_values.IsEmpty())
    return;
  // Attribute order doesn't matter. Sort for easy equality comparison.
  std::sort(result.attributes_and_values.begin(),
            result.attributes_and_values.end(), AttributeNameSort);
  // The cache key is non-null when the tagName is set.
  result.tag_name = element.localName().Impl();
}

static unsigned ComputePresentationAttributeCacheHash(
    const PresentationAttributeCacheKey& key) {
  if (!key.tag_name)
    return 0;
  DCHECK(key.attributes_and_values.size());
  unsigned attribute_hash = StringHasher::HashMemory(
      key.attributes_and_values.data(),
      key.attributes_and_values.size() * sizeof(key.attributes_and_values[0]));
  return WTF::HashInts(key.tag_name->ExistingHash(), attribute_hash);
}

CSSPropertyValueSet* ComputePresentationAttributeStyle(Element& element) {
  DCHECK(element.IsStyledElement());

  PresentationAttributeCacheKey cache_key;
  MakePresentationAttributeCacheKey(element, cache_key);

  unsigned cache_hash = ComputePresentationAttributeCacheHash(cache_key);

  PresentationAttributeCacheEntry* cache_entry = nullptr;

  PresentationAttributeCache &cache = GetPresentationAttributeCache();
  if (cache_hash) {
    auto it = cache.find(cache_hash);
    if (std::end(cache) != it) {
      cache_entry = it->second.Get();
      if (cache_entry->key != cache_key)
        cache_hash = 0;
    }
  }

  // Keep the entry value of |cache_value| here in order to assure that the
  // value lives when it is used. Without this keeping, calling
  // |GetPresentationAttributeCache().clear()| destroys |cache_value->value| and
  // causes use-after-poison (crbug.com/810368).

  CSSPropertyValueSet* style = nullptr;
  if (cache_hash && cache_entry) {
    style = cache_entry->value;

    static const unsigned kMinimumPresentationAttributeCacheSizeForCleaning =
        100;
    if (cache.size() >= kMinimumPresentationAttributeCacheSizeForCleaning)
      cache.clear();
  } else {
    style = MutableCSSPropertyValueSet::Create(kHTMLStandardMode);
    AttributeCollection attributes = element.AttributesWithoutUpdate();
    for (const Attribute& attr : attributes) {
      element.CollectStyleForPresentationAttribute(
          attr.GetName(), attr.Value(), ToMutableCSSPropertyValueSet(style));
    }
  }

  if (!cache_hash || cache_entry)
    return style;

  PresentationAttributeCacheEntry* new_entry =
      new PresentationAttributeCacheEntry;
  new_entry->key = cache_key;
  new_entry->value = style;

  static const unsigned kPresentationAttributeCacheMaximumSize = 4096;
  if (cache.size() > kPresentationAttributeCacheMaximumSize) {
    // FIXME: Discarding the entire cache when it gets too big is probably bad
    // since it creates a perf "cliff". Perhaps we should use an LRU?
    cache.clear();
  }
  cache[cache_hash] = new_entry;

  return style;
}

}  // namespace blink
