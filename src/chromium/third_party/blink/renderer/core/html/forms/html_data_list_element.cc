// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: html_data_list_element.cc
// Description: HTMLDataListElement Class
//      Author: Ziming Li
//     Created: 2021-01-05
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2009 Google Inc. All rights reserved.
 * Copyright (C) 2010 Apple Inc. All rights reserved.
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

#include "third_party/blink/renderer/core/html/forms/html_data_list_element.h"

#include "third_party/blink/renderer/core/dom/id_target_observer_registry.h"
#include "third_party/blink/renderer/core/dom/node_lists_node_data.h"
#include "third_party/blink/renderer/core/frame/use_counter.h"
// BKTODO: #include "third_party/blink/renderer/core/html/forms/html_data_list_options_collection.h"
#include "third_party/blink/renderer/core/html_names.h"

namespace blink {

inline HTMLDataListElement::HTMLDataListElement(Document& document)
    : HTMLElement(HTMLNames::datalistTag, document) {}

Element* HTMLDataListElement::Create(Document& document, const CreateElementFlags) {
  UseCounter::Count(document, WebFeature::kDataListElement);
  return new HTMLDataListElement(document);
}

HTMLDataListOptionsCollection* HTMLDataListElement::options() {
  ASSERT(false); // BKTODO:
  return nullptr;
#if 0
  return EnsureCachedCollection<HTMLDataListOptionsCollection>(
      kDataListOptions);
#endif
}

void HTMLDataListElement::ChildrenChanged(const ChildrenChange& change) {
  HTMLElement::ChildrenChanged(change);
  if (!change.by_parser) {
    ASSERT(false); // BKTODO:
#if 0
    GetTreeScope().GetIdTargetObserverRegistry().NotifyObservers(
        GetIdAttribute());
#endif
  }
}

void HTMLDataListElement::FinishParsingChildren() {
  ASSERT(false); // BKTODO:
#if 0
  GetTreeScope().GetIdTargetObserverRegistry().NotifyObservers(
      GetIdAttribute());
#endif
}

void HTMLDataListElement::OptionElementChildrenChanged() {
  ASSERT(false); // BKTODO:
#if 0
  GetTreeScope().GetIdTargetObserverRegistry().NotifyObservers(
      GetIdAttribute());
#endif
}

}  // namespace blink
