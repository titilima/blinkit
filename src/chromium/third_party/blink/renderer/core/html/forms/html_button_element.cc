// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: html_button_element.cc
// Description: HTMLButtonElement Class
//      Author: Ziming Li
//     Created: 2021-01-05
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2001 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2004, 2005, 2006, 2007, 2010 Apple Inc. All rights reserved.
 *           (C) 2006 Alexey Proskuryakov (ap@nypop.com)
 * Copyright (C) 2007 Samuel Weinig (sam@webkit.org)
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

#include "third_party/blink/renderer/core/html/forms/html_button_element.h"

#include "third_party/blink/renderer/core/dom/attribute.h"
// BKTODO: #include "third_party/blink/renderer/core/events/keyboard_event.h"
// BKTODO: #include "third_party/blink/renderer/core/html/forms/form_data.h"
// BKTODO: #include "third_party/blink/renderer/core/html/forms/html_form_element.h"
#include "third_party/blink/renderer/core/html_names.h"
#include "third_party/blink/renderer/core/layout/layout_button.h"
#include "third_party/blink/renderer/platform/wtf/std_lib_extras.h"

namespace blink {

using namespace HTMLNames;

inline HTMLButtonElement::HTMLButtonElement(Document& document)
    : HTMLFormControlElement(buttonTag, document),
      type_(SUBMIT),
      is_activated_submit_(false) {}

Element* HTMLButtonElement::Create(Document& document, const CreateElementFlags) {
  return new HTMLButtonElement(document);
}

void HTMLButtonElement::setType(const AtomicString& type) {
  setAttribute(typeAttr, type);
}

LayoutObject* HTMLButtonElement::CreateLayoutObject(const ComputedStyle&) {
  return new LayoutButton(this);
}

const AtomicString& HTMLButtonElement::FormControlType() const {
  switch (type_) {
    case SUBMIT: {
      DEFINE_STATIC_LOCAL(const AtomicString, submit, ("submit"));
      return submit;
    }
    case BUTTON: {
      DEFINE_STATIC_LOCAL(const AtomicString, button, ("button"));
      return button;
    }
    case RESET: {
      DEFINE_STATIC_LOCAL(const AtomicString, reset, ("reset"));
      return reset;
    }
  }

  NOTREACHED();
  return g_empty_atom;
}

bool HTMLButtonElement::IsPresentationAttribute(
    const QualifiedName& name) const {
  if (name == alignAttr) {
    // Don't map 'align' attribute.  This matches what Firefox and IE do, but
    // not Opera.  See http://bugs.webkit.org/show_bug.cgi?id=12071
    return false;
  }

  return HTMLFormControlElement::IsPresentationAttribute(name);
}

void HTMLButtonElement::ParseAttribute(
    const AttributeModificationParams& params) {
  if (params.name == typeAttr) {
    if (DeprecatedEqualIgnoringCase(params.new_value, "reset"))
      type_ = RESET;
    else if (DeprecatedEqualIgnoringCase(params.new_value, "button"))
      type_ = BUTTON;
    else
      type_ = SUBMIT;
    SetNeedsWillValidateCheck();
    if (formOwner() && isConnected())
      ASSERT(false); // BKTODO: formOwner()->InvalidateDefaultButtonStyle();
  } else {
    HTMLFormControlElement::ParseAttribute(params);
  }
}

void HTMLButtonElement::DefaultEventHandler(Event& event) {
  ASSERT(false); // BKTODO:
#if 0
  if (event.type() == EventTypeNames::DOMActivate && !IsDisabledFormControl()) {
    if (Form() && type_ == SUBMIT) {
      Form()->PrepareForSubmission(event, this);
      event.SetDefaultHandled();
    }
    if (Form() && type_ == RESET) {
      Form()->reset();
      event.SetDefaultHandled();
    }
  }

  if (event.IsKeyboardEvent()) {
    if (event.type() == EventTypeNames::keydown &&
        ToKeyboardEvent(event).key() == " ") {
      SetActive(true);
      // No setDefaultHandled() - IE dispatches a keypress in this case.
      return;
    }
    if (event.type() == EventTypeNames::keypress) {
      switch (ToKeyboardEvent(event).charCode()) {
        case '\r':
          DispatchSimulatedClick(&event);
          event.SetDefaultHandled();
          return;
        case ' ':
          // Prevent scrolling down the page.
          event.SetDefaultHandled();
          return;
      }
    }
    if (event.type() == EventTypeNames::keyup &&
        ToKeyboardEvent(event).key() == " ") {
      if (IsActive())
        DispatchSimulatedClick(&event);
      event.SetDefaultHandled();
      return;
    }
  }
#endif

  HTMLFormControlElement::DefaultEventHandler(event);
}

bool HTMLButtonElement::HasActivationBehavior() const {
  return true;
}

bool HTMLButtonElement::WillRespondToMouseClickEvents() {
  if (!IsDisabledFormControl() && Form() && (type_ == SUBMIT || type_ == RESET))
    return true;
  return HTMLFormControlElement::WillRespondToMouseClickEvents();
}

bool HTMLButtonElement::CanBeSuccessfulSubmitButton() const {
  return type_ == SUBMIT;
}

bool HTMLButtonElement::IsActivatedSubmit() const {
  return is_activated_submit_;
}

void HTMLButtonElement::SetActivatedSubmit(bool flag) {
  is_activated_submit_ = flag;
}

void HTMLButtonElement::AppendToFormData(FormData& form_data) {
  ASSERT(false); // BKTODO:
#if 0
  if (type_ == SUBMIT && !GetName().IsEmpty() && is_activated_submit_)
    form_data.AppendFromElement(GetName(), Value());
#endif
}

void HTMLButtonElement::AccessKeyAction(bool send_mouse_events) {
  ASSERT(false); // BKTODO:
#if 0
  focus();

  DispatchSimulatedClick(
      nullptr, send_mouse_events ? kSendMouseUpDownEvents : kSendNoEvents);
#endif
}

bool HTMLButtonElement::IsURLAttribute(const Attribute& attribute) const {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return attribute.GetName() == formactionAttr ||
         HTMLFormControlElement::IsURLAttribute(attribute);
#endif
}

const AtomicString& HTMLButtonElement::Value() const {
  ASSERT(false); // BKTODO:
  return g_null_atom;
#if 0
  return getAttribute(valueAttr);
#endif
}

bool HTMLButtonElement::RecalcWillValidate() const {
  return type_ == SUBMIT && HTMLFormControlElement::RecalcWillValidate();
}

bool HTMLButtonElement::IsInteractiveContent() const {
  return true;
}

bool HTMLButtonElement::SupportsAutofocus() const {
  return true;
}

bool HTMLButtonElement::MatchesDefaultPseudoClass() const {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  // HTMLFormElement::findDefaultButton() traverses the tree. So we check
  // canBeSuccessfulSubmitButton() first for early return.
  return CanBeSuccessfulSubmitButton() && Form() &&
         Form()->FindDefaultButton() == this;
#endif
}

Node::InsertionNotificationRequest HTMLButtonElement::InsertedInto(
    ContainerNode& insertion_point) {
  InsertionNotificationRequest request =
      HTMLFormControlElement::InsertedInto(insertion_point);
  return request;
}

}  // namespace blink
