// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: matched_properties_cache.h
// Description: MatchedPropertiesCache Class
//      Author: Ziming Li
//     Created: 2020-08-02
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 * Copyright (C) 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011 Apple Inc.
 * All rights reserved.
 * Copyright (C) 2013 Google Inc. All rights reserved.
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
 *
 */

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_CSS_RESOLVER_MATCHED_PROPERTIES_CACHE_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_CSS_RESOLVER_MATCHED_PROPERTIES_CACHE_H_

#include "base/macros.h"
#include "third_party/blink/renderer/core/css/css_property_value_set.h"
#include "third_party/blink/renderer/core/css/resolver/match_result.h"
#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/wtf/forward.h"
#include "third_party/blink/renderer/platform/wtf/hash_map.h"
#include "third_party/blink/renderer/platform/wtf/noncopyable.h"

namespace blink {

class ComputedStyle;
class StyleResolverState;

class CachedMatchedProperties final
    : public GarbageCollectedFinalized<CachedMatchedProperties> {
 public:
  HeapVector<MatchedProperties> matched_properties;
  scoped_refptr<ComputedStyle> computed_style;
  scoped_refptr<ComputedStyle> parent_computed_style;

  void Set(const ComputedStyle&,
           const ComputedStyle& parent_style,
           const MatchedPropertiesVector&);
  void Clear();
};

class CORE_EXPORT MatchedPropertiesCache {
  DISALLOW_NEW();

 public:
  MatchedPropertiesCache();
  ~MatchedPropertiesCache() { DCHECK(cache_.empty()); }

  const CachedMatchedProperties* Find(unsigned hash,
                                      const StyleResolverState&,
                                      const MatchedPropertiesVector&);
  void Add(const ComputedStyle&,
           const ComputedStyle& parent_style,
           unsigned hash,
           const MatchedPropertiesVector&);

  void Clear();
  void ClearViewportDependent();

  static bool IsCacheable(const StyleResolverState&);
  static bool IsStyleCacheable(const ComputedStyle&);

  void Trace(blink::Visitor*);

 private:
  using Cache = std::unordered_map<unsigned, CachedMatchedProperties>;
  Cache cache_;
  DISALLOW_COPY_AND_ASSIGN(MatchedPropertiesCache);
};

}  // namespace blink

#endif
