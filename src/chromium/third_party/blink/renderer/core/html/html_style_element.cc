// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: html_style_element.cc
// Description: HTMLStyleElement Class
//      Author: Ziming Li
//     Created: 2020-12-24
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2001 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2003, 2010 Apple Inc. All rights reserved.
 *           (C) 2007 Rob Buis (buis@kde.org)
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

#include "third_party/blink/renderer/core/html/html_style_element.h"

#include "third_party/blink/public/platform/task_type.h"
#include "third_party/blink/renderer/core/css/media_list.h"
#include "third_party/blink/renderer/core/css/style_engine.h"
#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/dom/events/event.h"
#include "third_party/blink/renderer/core/dom/shadow_root.h"
#include "third_party/blink/renderer/core/html_names.h"

namespace blink {

using namespace HTMLNames;

inline HTMLStyleElement::HTMLStyleElement(Document& document,
                                          const CreateElementFlags flags)
    : HTMLElement(styleTag, document),
      StyleElement(&document, flags.IsCreatedByParser()),
      fired_load_(false),
      loaded_sheet_(false) {}

HTMLStyleElement::~HTMLStyleElement() = default;

HTMLStyleElement* HTMLStyleElement::Create(Document& document,
                                           const CreateElementFlags flags) {
  return new HTMLStyleElement(document, flags);
}

void HTMLStyleElement::ParseAttribute(
    const AttributeModificationParams& params) {
  if (params.name == titleAttr && sheet_ && IsInDocumentTree()) {
    sheet_->SetTitle(params.new_value);
  } else if (params.name == mediaAttr && isConnected() &&
             GetDocument().IsActive() && sheet_) {
    sheet_->SetMediaQueries(MediaQuerySet::Create(params.new_value));
    GetDocument().GetStyleEngine().MediaQueriesChangedInScope(GetTreeScope());
  } else if (params.name == typeAttr) {
    HTMLElement::ParseAttribute(params);
    StyleElement::ChildrenChanged(*this);
  } else {
    HTMLElement::ParseAttribute(params);
  }
}

void HTMLStyleElement::FinishParsingChildren() {
  StyleElement::ProcessingResult result =
      StyleElement::FinishParsingChildren(*this);
  HTMLElement::FinishParsingChildren();
  if (result == StyleElement::kProcessingFatalError)
    NotifyLoadedSheetAndAllCriticalSubresources(
        kErrorOccurredLoadingSubresource);
}

Node::InsertionNotificationRequest HTMLStyleElement::InsertedInto(
    ContainerNode& insertion_point) {
  HTMLElement::InsertedInto(insertion_point);
  if (isConnected()) {
    if (StyleElement::ProcessStyleSheet(GetDocument(), *this) ==
        StyleElement::kProcessingFatalError) {
      NotifyLoadedSheetAndAllCriticalSubresources(
          kErrorOccurredLoadingSubresource);
    }
  }
  return kInsertionDone;
}

void HTMLStyleElement::RemovedFrom(ContainerNode& insertion_point) {
  HTMLElement::RemovedFrom(insertion_point);
  StyleElement::RemovedFrom(*this, insertion_point);
}

void HTMLStyleElement::ChildrenChanged(const ChildrenChange& change) {
  HTMLElement::ChildrenChanged(change);
  if (StyleElement::ChildrenChanged(*this) ==
      StyleElement::kProcessingFatalError)
    NotifyLoadedSheetAndAllCriticalSubresources(
        kErrorOccurredLoadingSubresource);
}

const AtomicString& HTMLStyleElement::media() const {
  return getAttribute(mediaAttr);
}

const AtomicString& HTMLStyleElement::type() const {
  return getAttribute(typeAttr);
}

void HTMLStyleElement::DispatchPendingEvent(
    std::unique_ptr<IncrementLoadEventDelayCount> count) {
  if (loaded_sheet_) {
    if (GetDocument().HasListenerType(
            Document::kLoadListenerAtCapturePhaseOrAtStyleElement))
      DispatchEvent(*Event::Create(event_type_names::kLoad));
  } else {
    DispatchEvent(*Event::Create(event_type_names::kError));
  }
  // Checks Document's load event synchronously here for performance.
  // This is safe because dispatchPendingEvent() is called asynchronously.
  count->ClearAndCheckLoadEvent();
}

void HTMLStyleElement::NotifyLoadedSheetAndAllCriticalSubresources(
    LoadedSheetErrorStatus error_status) {
  bool is_load_event = error_status == kNoErrorLoadingSubresource;
  if (fired_load_ && is_load_event)
    return;
  loaded_sheet_ = is_load_event;

  Document &document = GetDocument();
  auto count = IncrementLoadEventDelayCount::Create(document);
  auto task = [this, count = count.release()]
  {
    DispatchPendingEvent(base::WrapUnique(count));
  };
  document.GetTaskRunner(TaskType::kDOMManipulation)->PostTask(FROM_HERE, task);

  fired_load_ = true;
}

bool HTMLStyleElement::disabled() const {
  if (!sheet_)
    return false;

  return sheet_->disabled();
}

void HTMLStyleElement::setDisabled(bool set_disabled) {
  if (CSSStyleSheet* style_sheet = sheet())
    style_sheet->setDisabled(set_disabled);
}

void HTMLStyleElement::Trace(blink::Visitor* visitor) {
  StyleElement::Trace(visitor);
  HTMLElement::Trace(visitor);
}

}  // namespace blink
