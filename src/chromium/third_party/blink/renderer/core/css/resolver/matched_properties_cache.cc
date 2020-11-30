// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: matched_properties_cache.cc
// Description: MatchedPropertiesCache Class
//      Author: Ziming Li
//     Created: 2020-09-22
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 2004-2005 Allan Sandfeld Jensen (kde@carewolf.com)
 * Copyright (C) 2006, 2007 Nicholas Shanks (webkit@nickshanks.com)
 * Copyright (C) 2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013 Apple Inc.
 * All rights reserved.
 * Copyright (C) 2007 Alexey Proskuryakov <ap@webkit.org>
 * Copyright (C) 2007, 2008 Eric Seidel <eric@webkit.org>
 * Copyright (C) 2008, 2009 Torch Mobile Inc. All rights reserved.
 * (http://www.torchmobile.com/)
 * Copyright (c) 2011, Code Aurora Forum. All rights reserved.
 * Copyright (C) Research In Motion Limited 2011. All rights reserved.
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
 */

#include "third_party/blink/renderer/core/css/resolver/matched_properties_cache.h"

#include "third_party/blink/renderer/core/css/css_property_value_set.h"
#include "third_party/blink/renderer/core/css/resolver/style_resolver_state.h"
#include "third_party/blink/renderer/core/style/computed_style.h"

namespace blink {

void CachedMatchedProperties::Set(const ComputedStyle& style,
                                  const ComputedStyle& parent_style,
                                  const MatchedPropertiesVector& properties) {
  matched_properties.insert(matched_properties.end(), properties.begin(), properties.end());

  // Note that we don't cache the original ComputedStyle instance. It may be
  // further modified.  The ComputedStyle in the cache is really just a holder
  // for the substructures and never used as-is.
  this->computed_style = ComputedStyle::Clone(style);
  this->parent_computed_style = ComputedStyle::Clone(parent_style);
}

void CachedMatchedProperties::Clear() {
  matched_properties.clear();
  computed_style = nullptr;
  parent_computed_style = nullptr;
}

MatchedPropertiesCache::MatchedPropertiesCache() = default;

const CachedMatchedProperties* MatchedPropertiesCache::Find(
    unsigned hash,
    const StyleResolverState& style_resolver_state,
    const MatchedPropertiesVector& properties) {
  DCHECK(hash);

  Cache::iterator it = cache_.find(hash);
  if (it == cache_.end())
    return nullptr;
  ASSERT(false); // BKTODO:
  return nullptr;
#if 0
  CachedMatchedProperties* cache_item = it->second.Get();
  if (!cache_item)
    return nullptr;

  wtf_size_t size = properties.size();
  if (size != cache_item->matched_properties.size())
    return nullptr;
  if (cache_item->computed_style->InsideLink() !=
      style_resolver_state.Style()->InsideLink())
    return nullptr;
  for (wtf_size_t i = 0; i < size; ++i) {
    if (properties[i] != cache_item->matched_properties[i])
      return nullptr;
  }
  return cache_item;
#endif
}

void MatchedPropertiesCache::Add(const ComputedStyle& style,
                                 const ComputedStyle& parent_style,
                                 unsigned hash,
                                 const MatchedPropertiesVector& properties) {
  DCHECK(hash);
  Member<CachedMatchedProperties> &cache_item = cache_[hash];
  if (cache_item)
    cache_item->Clear();
  else
    cache_item = new CachedMatchedProperties;

  cache_item->Set(style, parent_style, properties);
}

void MatchedPropertiesCache::Clear() {
  // MatchedPropertiesCache must be cleared promptly because some
  // destructors in the properties (e.g., ~FontFallbackList) expect that
  // the destructors are called promptly without relying on a GC timing.
  for (auto& cache_entry : cache_) {
    if (cache_entry.second)
      cache_entry.second->Clear();
  }
  cache_.clear();
}

void MatchedPropertiesCache::ClearViewportDependent() {
  Vector<unsigned, 16> to_remove;
  ASSERT(false); // BKTODO:
#if 0
  for (const auto& cache_entry : cache_) {
    CachedMatchedProperties* cache_item = cache_entry.value.Get();
    if (cache_item && cache_item->computed_style->HasViewportUnits())
      to_remove.push_back(cache_entry.key);
  }
  cache_.RemoveAll(to_remove);
#endif
}

bool MatchedPropertiesCache::IsStyleCacheable(const ComputedStyle& style) {
  // Content property with attr() values depend on the attribute value of the
  // originating element, thus we cannot cache based on the matched properties
  // because the value of content is retrieved from the attribute at apply time.
  if (style.HasAttrContent())
    return false;
  if (style.Zoom() != ComputedStyleInitialValues::InitialZoom())
    return false;
  if (style.TextAutosizingMultiplier() != 1)
    return false;
  if (style.GetWritingMode() !=
          ComputedStyleInitialValues::InitialWritingMode() ||
      style.Direction() != ComputedStyleInitialValues::InitialDirection())
    return false;
  // styles with non inherited properties that reference variables are not
  // cacheable.
  if (style.HasVariableReferenceFromNonInheritedProperty())
    return false;
  return true;
}

bool MatchedPropertiesCache::IsCacheable(const StyleResolverState& state) {
  const ComputedStyle& style = *state.Style();
  const ComputedStyle& parent_style = *state.ParentStyle();

  if (!IsStyleCacheable(style))
    return false;
  // The cache assumes static knowledge about which properties are inherited.
  // Without a flat tree parent, StyleBuilder::ApplyProperty will not
  // SetHasExplicitlyInheritedProperties on the parent style.
  if (!state.ParentNode() || parent_style.HasExplicitlyInheritedProperties())
    return false;
  return true;
}

void MatchedPropertiesCache::Trace(blink::Visitor* visitor) {
  visitor->Trace(cache_);
}

}  // namespace blink
