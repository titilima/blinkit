// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: html_body_element.h
// Description: HTMLBodyElement Class
//      Author: Ziming Li
//     Created: 2020-11-02
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2000 Simon Hausmann <hausmann@kde.org>
 * Copyright (C) 2004, 2006, 2009, 2010 Apple Inc. All rights reserved.
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

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_HTML_HTML_BODY_ELEMENT_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_HTML_HTML_BODY_ELEMENT_H_

#include "third_party/blink/renderer/core/core_export.h"
#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/html/html_element.h"

namespace blink {

class Document;

class CORE_EXPORT HTMLBodyElement final : public HTMLElement {
  DEFINE_WRAPPERTYPEINFO();

 public:
  DECLARE_NODE_FACTORY(HTMLBodyElement);
  ~HTMLBodyElement() override;

#if 0 // BKTODO:
  DEFINE_WINDOW_ATTRIBUTE_EVENT_LISTENER(blur);
  DEFINE_WINDOW_ATTRIBUTE_EVENT_LISTENER(error);
  DEFINE_WINDOW_ATTRIBUTE_EVENT_LISTENER(focus);
  DEFINE_WINDOW_ATTRIBUTE_EVENT_LISTENER(load);
  DEFINE_WINDOW_ATTRIBUTE_EVENT_LISTENER(resize);
  DEFINE_WINDOW_ATTRIBUTE_EVENT_LISTENER(scroll);
  DEFINE_WINDOW_ATTRIBUTE_EVENT_LISTENER(orientationchange);
#endif

 private:
  explicit HTMLBodyElement(Document&);

  void ParseAttribute(const AttributeModificationParams&) override;
  bool IsPresentationAttribute(const QualifiedName&) const override;
  void CollectStyleForPresentationAttribute(
      const QualifiedName&,
      const AtomicString&,
      MutableCSSPropertyValueSet*) override;

  InsertionNotificationRequest InsertedInto(ContainerNode&) override;
  void DidNotifySubtreeInsertionsToDocument() override;

  bool IsURLAttribute(const Attribute&) const override;
  bool HasLegalLinkAttribute(const QualifiedName&) const override;
  const QualifiedName& SubResourceAttributeName() const override;

  bool SupportsFocus() const override;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_HTML_HTML_BODY_ELEMENT_H_