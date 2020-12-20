// -------------------------------------------------
// BlinKit - cc Library
// -------------------------------------------------
//   File Name: element_id.cc
// Description: ElementId Class
//      Author: Ziming Li
//     Created: 2020-12-20
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "cc/trees/element_id.h"

#include <inttypes.h>
#include <limits>
#include <ostream>

#include "base/strings/stringprintf.h"
#include "base/trace_event/trace_event_argument.h"
// BKTODO: #include "base/values.h"

namespace cc {

bool ElementId::operator==(const ElementId& o) const {
  return id_ == o.id_;
}

bool ElementId::operator!=(const ElementId& o) const {
  return !(*this == o);
}

bool ElementId::operator<(const ElementId& o) const {
  return id_ < o.id_;
}

ElementId::operator bool() const {
  return !!id_;
}

ElementId LayerIdToElementIdForTesting(int layer_id) {
  return ElementId(std::numeric_limits<int>::max() - layer_id);
}

void ElementId::AddToTracedValue(base::trace_event::TracedValue* res) const {
  ASSERT(false); // BKTODO:
#if 0
  res->BeginDictionary("element_id");
  res->SetInteger("id_", id_);
  res->EndDictionary();
#endif
}

ElementIdType ElementId::GetInternalValue() const {
  return id_;
}

std::string ElementId::ToString() const {
  return base::StringPrintf("(%" PRIu64 ")", id_);
}

#if 0 // BKTODO:
std::unique_ptr<base::Value> ElementId::AsValue() const {
  std::unique_ptr<base::DictionaryValue> res(new base::DictionaryValue());
  res->SetInteger("id_", id_);
  return std::move(res);
}
#endif

size_t ElementIdHash::operator()(ElementId key) const {
  return std::hash<int>()(key.id_);
}

std::ostream& operator<<(std::ostream& out, const ElementId& id) {
  return out << id.ToString();
}

}  // namespace cc
