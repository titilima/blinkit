// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: nth_index_cache.h
// Description: NthIndexCache Class
//      Author: Ziming Li
//     Created: 2020-03-28
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_DOM_NTH_INDEX_CACHE_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_DOM_NTH_INDEX_CACHE_H_

#include <unordered_map>
#include "base/macros.h"
#include "third_party/blink/renderer/core/core_export.h"
#include "third_party/blink/renderer/core/dom/element.h"
#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/wtf/hash_map.h"

namespace blink {

class Document;

class CORE_EXPORT NthIndexData final : public GarbageCollected<NthIndexData> {
 public:
  NthIndexData(ContainerNode&);
  NthIndexData(ContainerNode&, const QualifiedName& type);

  unsigned NthIndex(Element&) const;
  unsigned NthLastIndex(Element&) const;
  unsigned NthOfTypeIndex(Element&) const;
  unsigned NthLastOfTypeIndex(Element&) const;

 private:
  std::unordered_map<Element *, unsigned> element_index_map_;
  unsigned count_ = 0;
  DISALLOW_COPY_AND_ASSIGN(NthIndexData);
};

class CORE_EXPORT NthIndexCache final {
  STACK_ALLOCATED();

 public:
  explicit NthIndexCache(Document&);
  ~NthIndexCache();

  static unsigned NthChildIndex(Element&);
  static unsigned NthLastChildIndex(Element&);
  static unsigned NthOfTypeIndex(Element&);
  static unsigned NthLastOfTypeIndex(Element&);

 private:
  using IndexByType = std::unordered_map<String, std::unique_ptr<NthIndexData>>;
  using ParentMap = std::unordered_map<Node *, std::unique_ptr<NthIndexData>>;
  using ParentMapForType = std::unordered_map<Node *, std::unique_ptr<IndexByType>>;

  void CacheNthIndexDataForParent(Element&);
  void CacheNthOfTypeIndexDataForParent(Element&);
  IndexByType& EnsureTypeIndexMap(ContainerNode&);
  NthIndexData* NthTypeIndexDataForParent(Element&) const;

  Member<Document> document_;
  Member<ParentMap> parent_map_;
  Member<ParentMapForType> parent_map_for_type_;

#if DCHECK_IS_ON()
  uint64_t dom_tree_version_;
#endif
  DISALLOW_COPY_AND_ASSIGN(NthIndexCache);
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_DOM_NTH_INDEX_CACHE_H_
