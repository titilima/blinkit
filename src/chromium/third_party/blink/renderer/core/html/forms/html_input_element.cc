// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: html_input_element.cc
// Description: HTMLInputElement Class
//      Author: Ziming Li
//     Created: 2021-01-06
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2001 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011 Apple Inc. All
 * rights reserved.
 *           (C) 2006 Alexey Proskuryakov (ap@nypop.com)
 * Copyright (C) 2007 Samuel Weinig (sam@webkit.org)
 * Copyright (C) 2010 Google Inc. All rights reserved.
 * Copyright (C) 2008 Torch Mobile Inc. All rights reserved.
 * (http://www.torchmobile.com/)
 * Copyright (C) 2012 Samsung Electronics. All rights reserved.
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

#include "third_party/blink/renderer/core/html/forms/html_input_element.h"

#include "third_party/blink/public/platform/task_type.h"
#if 0 // BKTODO:
#include "third_party/blink/public/platform/web_scroll_into_view_params.h"
#include "third_party/blink/renderer/bindings/core/v8/script_event_listener.h"
#include "third_party/blink/renderer/core/accessibility/ax_object_cache.h"
#endif
#include "third_party/blink/renderer/core/css/style_change_reason.h"
#include "third_party/blink/renderer/core/css_property_names.h"
#include "third_party/blink/renderer/core/dom/document.h"
// BKTODO: #include "third_party/blink/renderer/core/dom/events/scoped_event_queue.h"
#include "third_party/blink/renderer/core/dom/id_target_observer.h"
#include "third_party/blink/renderer/core/dom/shadow_root.h"
#include "third_party/blink/renderer/core/dom/v0_insertion_point.h"
#include "third_party/blink/renderer/core/editing/frame_selection.h"
#if 0 // BKTODO:
#include "third_party/blink/renderer/core/editing/spellcheck/spell_checker.h"
#include "third_party/blink/renderer/core/events/before_text_inserted_event.h"
#include "third_party/blink/renderer/core/events/keyboard_event.h"
#include "third_party/blink/renderer/core/events/mouse_event.h"
#endif
#include "third_party/blink/renderer/core/frame/deprecation.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/frame/local_frame_view.h"
#include "third_party/blink/renderer/core/frame/use_counter.h"
#if 0 // BKTODO:
#include "third_party/blink/renderer/core/html/forms/color_chooser.h"
#include "third_party/blink/renderer/core/html/forms/date_time_chooser.h"
#include "third_party/blink/renderer/core/html/forms/file_input_type.h"
#endif
#include "third_party/blink/renderer/core/html/forms/form_controller.h"
#include "third_party/blink/renderer/core/html/forms/html_data_list_element.h"
#if 0 // BKTODO:
#include "third_party/blink/renderer/core/html/forms/html_data_list_options_collection.h"
#include "third_party/blink/renderer/core/html/forms/html_form_element.h"
#include "third_party/blink/renderer/core/html/forms/html_option_element.h"
#include "third_party/blink/renderer/core/html/forms/input_type.h"
#include "third_party/blink/renderer/core/html/forms/search_input_type.h"
#endif
#include "third_party/blink/renderer/core/html/html_collection.h"
// BKTODO: #include "third_party/blink/renderer/core/html/html_image_loader.h"
#include "third_party/blink/renderer/core/html/parser/html_parser_idioms.h"
#include "third_party/blink/renderer/core/html_names.h"
#include "third_party/blink/renderer/core/input_type_names.h"
#include "third_party/blink/renderer/core/layout/layout_object.h"
#include "third_party/blink/renderer/core/page/chrome_client.h"
#include "third_party/blink/renderer/core/page/page.h"
#include "third_party/blink/renderer/platform/bindings/exception_messages.h"
#include "third_party/blink/renderer/platform/bindings/exception_state.h"
#include "third_party/blink/renderer/platform/language.h"
#include "third_party/blink/renderer/platform/runtime_enabled_features.h"
#include "third_party/blink/renderer/platform/text/platform_locale.h"
#include "third_party/blink/renderer/platform/wtf/math_extras.h"

namespace blink {

// BKTODO: using ValueMode = InputType::ValueMode;
using namespace HTMLNames;

#if 0 // BKTODO:
class ListAttributeTargetObserver : public IdTargetObserver {
 public:
  static ListAttributeTargetObserver* Create(const AtomicString& id,
                                             HTMLInputElement*);
  void Trace(blink::Visitor*) override;
  void IdTargetChanged() override;

 private:
  ListAttributeTargetObserver(const AtomicString& id, HTMLInputElement*);

  Member<HTMLInputElement> element_;
};
#endif

const int kDefaultSize = 20;

HTMLInputElement::HTMLInputElement(Document& document,
                                   const CreateElementFlags flags)
    : TextControlElement(inputTag, document),
      size_(kDefaultSize),
      has_dirty_value_(false),
      is_checked_(false),
      dirty_checkedness_(false),
      is_indeterminate_(false),
      is_activated_submit_(false),
      autocomplete_(kUninitialized),
      has_non_empty_list_(false),
      state_restored_(false),
      parsing_in_progress_(flags.IsCreatedByParser()),
      can_receive_dropped_files_(false),
      should_reveal_password_(false),
      needs_to_update_view_value_(true),
      is_placeholder_visible_(false),
#if 0 // BKTODO:
      has_been_password_field_(false),
    // |input_type_| is lazily created when constructed by the parser to avoid
      // constructing unnecessarily a text InputType and its shadow subtree,
      // just to destroy them when the |type| attribute gets set by the parser
      // to something else than 'text'.
      input_type_(flags.IsCreatedByParser() ? nullptr
                                            : InputType::CreateText(*this)),
      input_type_view_(input_type_ ? input_type_->CreateView() : nullptr) {
#else
    has_been_password_field_(false) {
#endif
  SetHasCustomStyleCallbacks();
}

HTMLInputElement* HTMLInputElement::Create(Document& document,
                                           const CreateElementFlags flags) {
  auto* input_element = new HTMLInputElement(document, flags);
  if (!flags.IsCreatedByParser()) {
    ASSERT(false); // BKTODO:
#if 0
    DCHECK(input_element->input_type_view_->NeedsShadowSubtree());
    input_element->CreateUserAgentShadowRoot();
    input_element->CreateShadowSubtree();
#endif
  }
  return input_element;
}

void HTMLInputElement::Trace(blink::Visitor* visitor) {
  visitor->Trace(input_type_);
  visitor->Trace(input_type_view_);
  visitor->Trace(list_attribute_target_observer_);
  visitor->Trace(image_loader_);
  TextControlElement::Trace(visitor);
}

const HashSet<AtomicString>& HTMLInputElement::GetCheckedAttributeNames()
    const {
  DEFINE_STATIC_LOCAL(HashSet<AtomicString>, attribute_set, ({"src"}));
  return attribute_set;
}

#if 0 // BKTODO:
bool HTMLInputElement::HasPendingActivity() const {
  return ImageLoader() && ImageLoader()->HasPendingActivity();
}
#endif

HTMLImageLoader& HTMLInputElement::EnsureImageLoader() {
  if (!image_loader_)
    ASSERT(false); // BKTODO: image_loader_ = HTMLImageLoader::Create(this);
  return *image_loader_;
}

HTMLInputElement::~HTMLInputElement() = default;

const AtomicString& HTMLInputElement::GetName() const {
  return name_.IsNull() ? g_empty_atom : name_;
}

#if 0 // BKTODO:
FileChooserFileInfoList HTMLInputElement::FilesFromFileInputFormControlState(
    const FormControlState& state) {
  return FileInputType::FilesFromFormControlState(state);
}
#endif

bool HTMLInputElement::ShouldAutocomplete() const {
  if (autocomplete_ != kUninitialized)
    return autocomplete_ == kOn;
  return TextControlElement::ShouldAutocomplete();
}

bool HTMLInputElement::IsValidValue(const String& value) const {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  if (!input_type_->CanSetStringValue()) {
    NOTREACHED();
    return false;
  }
  return !input_type_->TypeMismatchFor(value) &&
         !input_type_->StepMismatch(value) &&
         !input_type_->RangeUnderflow(value) &&
         !input_type_->RangeOverflow(value) &&
         !TooLong(value, kIgnoreDirtyFlag) &&
         !TooShort(value, kIgnoreDirtyFlag) &&
         !input_type_->PatternMismatch(value) &&
         !input_type_->ValueMissing(value);
#endif
}

bool HTMLInputElement::TooLong() const {
  return willValidate() && TooLong(value(), kCheckDirtyFlag);
}

bool HTMLInputElement::TooShort() const {
  return willValidate() && TooShort(value(), kCheckDirtyFlag);
}

bool HTMLInputElement::TypeMismatch() const {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return willValidate() && input_type_->TypeMismatch();
#endif
}

bool HTMLInputElement::ValueMissing() const {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return willValidate() && input_type_->ValueMissing(value());
#endif
}

bool HTMLInputElement::HasBadInput() const {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return willValidate() && input_type_view_->HasBadInput();
#endif
}

bool HTMLInputElement::PatternMismatch() const {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return willValidate() && input_type_->PatternMismatch(value());
#endif
}

bool HTMLInputElement::TooLong(const String& value,
                               NeedsToCheckDirtyFlag check) const {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return input_type_->TooLong(value, check);
#endif
}

bool HTMLInputElement::TooShort(const String& value,
                                NeedsToCheckDirtyFlag check) const {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return input_type_->TooShort(value, check);
#endif
}

bool HTMLInputElement::RangeUnderflow() const {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return willValidate() && input_type_->RangeUnderflow(value());
#endif
}

bool HTMLInputElement::RangeOverflow() const {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return willValidate() && input_type_->RangeOverflow(value());
#endif
}

String HTMLInputElement::validationMessage() const {
  if (!willValidate())
    return String();

  if (CustomError())
    return CustomValidationMessage();

  ASSERT(false); // BKTODO:
  return String();
#if 0
  return input_type_->ValidationMessage(*input_type_view_).first;
#endif
}

String HTMLInputElement::ValidationSubMessage() const {
  if (!willValidate() || CustomError())
    return String();
  ASSERT(false); // BKTODO:
  return String();
#if 0
  return input_type_->ValidationMessage(*input_type_view_).second;
#endif
}

double HTMLInputElement::Minimum() const {
  ASSERT(false); // BKTODO:
  return 0;
#if 0
  return input_type_->Minimum();
#endif
}

double HTMLInputElement::Maximum() const {
  ASSERT(false); // BKTODO:
  return 0;
#if 0
  return input_type_->Maximum();
#endif
}

bool HTMLInputElement::StepMismatch() const {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return willValidate() && input_type_->StepMismatch(value());
#endif
}

#if 0 // BKTODO:
bool HTMLInputElement::GetAllowedValueStep(Decimal* step) const {
  return input_type_->GetAllowedValueStep(step);
}

StepRange HTMLInputElement::CreateStepRange(
    AnyStepHandling any_step_handling) const {
  return input_type_->CreateStepRange(any_step_handling);
}

Decimal HTMLInputElement::FindClosestTickMarkValue(const Decimal& value) {
  return input_type_->FindClosestTickMarkValue(value);
}
#endif

void HTMLInputElement::stepUp(int n, ExceptionState& exception_state) {
  ASSERT(false); // BKTODO:
#if 0
  input_type_->StepUp(n, exception_state);
#endif
}

void HTMLInputElement::stepDown(int n, ExceptionState& exception_state) {
  ASSERT(false); // BKTODO:
#if 0
  input_type_->StepUp(-1.0 * n, exception_state);
#endif
}

void HTMLInputElement::blur() {
  ASSERT(false); // BKTODO:
#if 0
  input_type_view_->Blur();
#endif
}

void HTMLInputElement::DefaultBlur() {
  TextControlElement::blur();
}

bool HTMLInputElement::HasCustomFocusLogic() const {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return input_type_view_->HasCustomFocusLogic();
#endif
}

bool HTMLInputElement::IsKeyboardFocusable() const {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return input_type_->IsKeyboardFocusable();
#endif
}

bool HTMLInputElement::MayTriggerVirtualKeyboard() const {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return input_type_->MayTriggerVirtualKeyboard();
#endif
}

void HTMLInputElement::UpdateFocusAppearanceWithOptions(
    SelectionBehaviorOnFocus selection_behavior,
    const FocusOptions& options) {
  if (IsTextField()) {
    switch (selection_behavior) {
      case SelectionBehaviorOnFocus::kReset:
        select();
        break;
      case SelectionBehaviorOnFocus::kRestore:
        RestoreCachedSelection();
        break;
      case SelectionBehaviorOnFocus::kNone:
        return;
    }
    ASSERT(false); // BKTODO:
#if 0
    // TODO(tkent): scrollRectToVisible is a workaround of a bug of
    // FrameSelection::revealSelection().  It doesn't scroll correctly in a
    // case of RangeSelection. crbug.com/443061.
    GetDocument().EnsurePaintLocationDataValidForNode(this);
    if (!options.preventScroll()) {
      if (GetLayoutObject()) {
        GetLayoutObject()->ScrollRectToVisible(BoundingBoxForScrollIntoView(),
                                               WebScrollIntoViewParams());
      }
      if (GetDocument().GetFrame())
        GetDocument().GetFrame()->Selection().RevealSelection();
    }
#endif
  } else {
    TextControlElement::UpdateFocusAppearanceWithOptions(selection_behavior,
                                                         options);
  }
}

void HTMLInputElement::EndEditing() {
  DCHECK(GetDocument().IsActive());
  if (!GetDocument().IsActive())
    return;

  if (!IsTextField())
    return;

  LocalFrame* frame = GetDocument().GetFrame();
#if 0 // BKTODO: Support spell checker?
  frame->GetSpellChecker().DidEndEditingOnTextField(this);
#endif
  frame->GetPage()->GetChromeClient().DidEndEditingOnTextField(*this);
}

void HTMLInputElement::DispatchFocusInEvent(
    const AtomicString& event_type,
    Element* old_focused_element,
    WebFocusType type,
    InputDeviceCapabilities* source_capabilities) {
  ASSERT(false); // BKTODO:
#if 0
  if (event_type == EventTypeNames::DOMFocusIn)
    input_type_view_->HandleFocusInEvent(old_focused_element, type);
#endif
  HTMLFormControlElementWithState::DispatchFocusInEvent(
      event_type, old_focused_element, type, source_capabilities);
}

void HTMLInputElement::HandleBlurEvent() {
  ASSERT(false); // BKTODO: input_type_view_->HandleBlurEvent();
}

void HTMLInputElement::setType(const AtomicString& type) {
  setAttribute(typeAttr, type);
}

void HTMLInputElement::InitializeTypeInParsing() {
  DCHECK(parsing_in_progress_);
  DCHECK(!input_type_);
  DCHECK(!input_type_view_);

  ASSERT(false); // BKTODO:
#if 0
  const AtomicString& new_type_name =
      InputType::NormalizeTypeName(FastGetAttribute(typeAttr));
  input_type_ = InputType::Create(*this, new_type_name);
  input_type_view_ = input_type_->CreateView();
  String default_value = FastGetAttribute(valueAttr);
  if (input_type_->GetValueMode() == ValueMode::kValue)
    non_attribute_value_ = SanitizeValue(default_value);
  has_been_password_field_ |= new_type_name == InputTypeNames::password;

  if (input_type_view_->NeedsShadowSubtree()) {
    CreateUserAgentShadowRoot();
    CreateShadowSubtree();
  }

  SetNeedsWillValidateCheck();

  if (!default_value.IsNull())
    input_type_->WarnIfValueIsInvalid(default_value);

  input_type_view_->UpdateView();
#endif
}

void HTMLInputElement::UpdateType() {
  DCHECK(input_type_);
  DCHECK(input_type_view_);

  ASSERT(false); // BKTODO:
#if 0
  const AtomicString& new_type_name =
      InputType::NormalizeTypeName(FastGetAttribute(typeAttr));
  if (input_type_->FormControlType() == new_type_name)
    return;

  InputType* new_type = InputType::Create(*this, new_type_name);
  RemoveFromRadioButtonGroup();

  ValueMode old_value_mode = input_type_->GetValueMode();
  bool did_respect_height_and_width =
      input_type_->ShouldRespectHeightAndWidthAttributes();
  bool could_be_successful_submit_button = CanBeSuccessfulSubmitButton();

  input_type_view_->DestroyShadowSubtree();
  DropInnerEditorElement();
  LazyReattachIfAttached();

  if (input_type_->SupportsRequired() != new_type->SupportsRequired() &&
      IsRequired()) {
    PseudoStateChanged(CSSSelector::kPseudoRequired);
    PseudoStateChanged(CSSSelector::kPseudoOptional);
  }
  if (input_type_->SupportsReadOnly() != new_type->SupportsReadOnly()) {
    PseudoStateChanged(CSSSelector::kPseudoReadOnly);
    PseudoStateChanged(CSSSelector::kPseudoReadWrite);
  }
  if (input_type_->IsCheckable() != new_type->IsCheckable()) {
    PseudoStateChanged(CSSSelector::kPseudoChecked);
  }
  PseudoStateChanged(CSSSelector::kPseudoIndeterminate);
  if (input_type_->IsSteppable() || new_type->IsSteppable()) {
    PseudoStateChanged(CSSSelector::kPseudoInRange);
    PseudoStateChanged(CSSSelector::kPseudoOutOfRange);
  }

  bool placeholder_changed =
      input_type_->SupportsPlaceholder() != new_type->SupportsPlaceholder();

  has_been_password_field_ |= new_type_name == InputTypeNames::password;

  input_type_ = new_type;
  input_type_view_ = input_type_->CreateView();
  if (input_type_view_->NeedsShadowSubtree()) {
    EnsureUserAgentShadowRoot();
    CreateShadowSubtree();
  }

  SetNeedsWillValidateCheck();

  if (placeholder_changed) {
    // We need to update the UA shadow and then the placeholder visibility flag
    // here. Otherwise it would happen as part of attaching the layout tree
    // which would be too late in order to make style invalidation work for
    // the upcoming frame.
    UpdatePlaceholderText();
    UpdatePlaceholderVisibility();
    PseudoStateChanged(CSSSelector::kPseudoPlaceholderShown);
  }

  ValueMode new_value_mode = input_type_->GetValueMode();

  // https://html.spec.whatwg.org/multipage/forms.html#input-type-change
  //
  // 1. If the previous state of the element's type attribute put the value IDL
  // attribute in the value mode, and the element's value is not the empty
  // string, and the new state of the element's type attribute puts the value
  // IDL attribute in either the default mode or the default/on mode, then set
  // the element's value content attribute to the element's value.
  if (old_value_mode == ValueMode::kValue &&
      (new_value_mode == ValueMode::kDefault ||
       new_value_mode == ValueMode::kDefaultOn)) {
    if (HasDirtyValue())
      setAttribute(valueAttr, AtomicString(non_attribute_value_));
    non_attribute_value_ = String();
    has_dirty_value_ = false;
  }
  // 2. Otherwise, if the previous state of the element's type attribute put the
  // value IDL attribute in any mode other than the value mode, and the new
  // state of the element's type attribute puts the value IDL attribute in the
  // value mode, then set the value of the element to the value of the value
  // content attribute, if there is one, or the empty string otherwise, and then
  // set the control's dirty value flag to false.
  else if (old_value_mode != ValueMode::kValue &&
           new_value_mode == ValueMode::kValue) {
    AtomicString value_string = FastGetAttribute(valueAttr);
    input_type_->WarnIfValueIsInvalid(value_string);
    non_attribute_value_ = SanitizeValue(value_string);
    has_dirty_value_ = false;
  }
  // 3. Otherwise, if the previous state of the element's type attribute put the
  // value IDL attribute in any mode other than the filename mode, and the new
  // state of the element's type attribute puts the value IDL attribute in the
  // filename mode, then set the value of the element to the empty string.
  else if (old_value_mode != ValueMode::kFilename &&
           new_value_mode == ValueMode::kFilename) {
    non_attribute_value_ = String();
    has_dirty_value_ = false;

  } else {
    // ValueMode wasn't changed, or kDefault <-> kDefaultOn.
    if (!HasDirtyValue()) {
      String default_value = FastGetAttribute(valueAttr);
      if (!default_value.IsNull())
        input_type_->WarnIfValueIsInvalid(default_value);
    }

    if (new_value_mode == ValueMode::kValue) {
      String new_value = SanitizeValue(non_attribute_value_);
      if (!EqualIgnoringNullity(new_value, non_attribute_value_)) {
        if (HasDirtyValue())
          setValue(new_value);
        else
          SetNonDirtyValue(new_value);
      }
    }
  }

  needs_to_update_view_value_ = true;
  input_type_view_->UpdateView();

  if (did_respect_height_and_width !=
      input_type_->ShouldRespectHeightAndWidthAttributes()) {
    DCHECK(GetElementData());
    AttributeCollection attributes = AttributesWithoutUpdate();
    if (const Attribute* height = attributes.Find(heightAttr)) {
      TextControlElement::AttributeChanged(AttributeModificationParams(
          heightAttr, height->Value(), height->Value(),
          AttributeModificationReason::kDirectly));
    }
    if (const Attribute* width = attributes.Find(widthAttr)) {
      TextControlElement::AttributeChanged(
          AttributeModificationParams(widthAttr, width->Value(), width->Value(),
                                      AttributeModificationReason::kDirectly));
    }
    if (const Attribute* align = attributes.Find(alignAttr)) {
      TextControlElement::AttributeChanged(
          AttributeModificationParams(alignAttr, align->Value(), align->Value(),
                                      AttributeModificationReason::kDirectly));
    }
  }

  // UA Shadow tree was recreated. We need to set selection again. We do it
  // later in order to avoid force layout.
  if (GetDocument().FocusedElement() == this)
    GetDocument().UpdateFocusAppearanceLater();

  // TODO(tkent): Should we dispatch a change event?
  ClearValueBeforeFirstUserEdit();

  AddToRadioButtonGroup();

  SetNeedsValidityCheck();
  if ((could_be_successful_submit_button || CanBeSuccessfulSubmitButton()) &&
      formOwner() && isConnected())
    formOwner()->InvalidateDefaultButtonStyle();
#endif
  NotifyFormStateChanged();
}

void HTMLInputElement::SubtreeHasChanged() {
  ASSERT(false); // BKTODO:
#if 0
  input_type_view_->SubtreeHasChanged();
  // When typing in an input field, childrenChanged is not called, so we need to
  // force the directionality check.
  CalculateAndAdjustDirectionality();
#endif
}

const AtomicString& HTMLInputElement::FormControlType() const {
  ASSERT(false); // BKTODO:
  return g_null_atom;
#if 0
  return input_type_->FormControlType();
#endif
}

bool HTMLInputElement::ShouldSaveAndRestoreFormControlState() const {
  ASSERT(false); // BKTODO:
#if 0
  if (!input_type_->ShouldSaveAndRestoreFormControlState())
    return false;
#endif
  return TextControlElement::ShouldSaveAndRestoreFormControlState();
}

FormControlState HTMLInputElement::SaveFormControlState() const {
  ASSERT(false); // BKTODO:
  return FormControlState();
#if 0
  return input_type_view_->SaveFormControlState();
#endif
}

void HTMLInputElement::RestoreFormControlState(const FormControlState& state) {
  ASSERT(false); // BKTODO: input_type_view_->RestoreFormControlState(state);
  state_restored_ = true;
}

bool HTMLInputElement::CanStartSelection() const {
  if (!IsTextField())
    return false;
  return TextControlElement::CanStartSelection();
}

unsigned HTMLInputElement::selectionStartForBinding(
    bool& is_null,
    ExceptionState& exception_state) const {
  ASSERT(false); // BKTODO:
#if 0
  if (!input_type_->SupportsSelectionAPI()) {
    is_null = true;
    return 0;
  }
#endif
  return TextControlElement::selectionStart();
}

unsigned HTMLInputElement::selectionEndForBinding(
    bool& is_null,
    ExceptionState& exception_state) const {
  ASSERT(false); // BKTODO:
#if 0
  if (!input_type_->SupportsSelectionAPI()) {
    is_null = true;
    return 0;
  }
#endif
  return TextControlElement::selectionEnd();
}

String HTMLInputElement::selectionDirectionForBinding(
    ExceptionState& exception_state) const {
  ASSERT(false); // BKTODO:
#if 0
  if (!input_type_->SupportsSelectionAPI()) {
    return String();
  }
#endif
  return TextControlElement::selectionDirection();
}

void HTMLInputElement::setSelectionStartForBinding(
    unsigned start,
    bool is_null,
    ExceptionState& exception_state) {
  ASSERT(false); // BKTODO:
#if 0
  if (!input_type_->SupportsSelectionAPI()) {
    exception_state.ThrowDOMException(DOMExceptionCode::kInvalidStateError,
                                      "The input element's type ('" +
                                          input_type_->FormControlType() +
                                          "') does not support selection.");
    return;
  }
#endif
  TextControlElement::setSelectionStart(start);
}

void HTMLInputElement::setSelectionEndForBinding(
    unsigned end,
    bool is_null,
    ExceptionState& exception_state) {
  ASSERT(false); // BKTODO:
#if 0
  if (!input_type_->SupportsSelectionAPI()) {
    exception_state.ThrowDOMException(DOMExceptionCode::kInvalidStateError,
                                      "The input element's type ('" +
                                          input_type_->FormControlType() +
                                          "') does not support selection.");
    return;
  }
#endif
  TextControlElement::setSelectionEnd(end);
}

void HTMLInputElement::setSelectionDirectionForBinding(
    const String& direction,
    ExceptionState& exception_state) {
  ASSERT(false); // BKTODO:
#if 0
  if (!input_type_->SupportsSelectionAPI()) {
    exception_state.ThrowDOMException(DOMExceptionCode::kInvalidStateError,
                                      "The input element's type ('" +
                                          input_type_->FormControlType() +
                                          "') does not support selection.");
    return;
  }
#endif
  TextControlElement::setSelectionDirection(direction);
}

void HTMLInputElement::setSelectionRangeForBinding(
    unsigned start,
    unsigned end,
    ExceptionState& exception_state) {
  ASSERT(false); // BKTODO:
#if 0
  if (!input_type_->SupportsSelectionAPI()) {
    exception_state.ThrowDOMException(DOMExceptionCode::kInvalidStateError,
                                      "The input element's type ('" +
                                          input_type_->FormControlType() +
                                          "') does not support selection.");
    return;
  }
#endif
  TextControlElement::setSelectionRangeForBinding(start, end);
}

void HTMLInputElement::setSelectionRangeForBinding(
    unsigned start,
    unsigned end,
    const String& direction,
    ExceptionState& exception_state) {
  ASSERT(false); // BKTODO:
#if 0
  if (!input_type_->SupportsSelectionAPI()) {
    exception_state.ThrowDOMException(DOMExceptionCode::kInvalidStateError,
                                      "The input element's type ('" +
                                          input_type_->FormControlType() +
                                          "') does not support selection.");
    return;
  }
#endif
  TextControlElement::setSelectionRangeForBinding(start, end, direction);
}

void HTMLInputElement::AccessKeyAction(bool send_mouse_events) {
  ASSERT(false); // BKTODO: input_type_view_->AccessKeyAction(send_mouse_events);
}

bool HTMLInputElement::IsPresentationAttribute(
    const QualifiedName& name) const {
  ASSERT(false); // BKTODO:
#if 0
  // FIXME: Remove type check.
  if (name == vspaceAttr || name == hspaceAttr || name == alignAttr ||
      name == widthAttr || name == heightAttr ||
      (name == borderAttr && type() == InputTypeNames::image))
    return true;
#endif
  return TextControlElement::IsPresentationAttribute(name);
}

void HTMLInputElement::CollectStyleForPresentationAttribute(
    const QualifiedName& name,
    const AtomicString& value,
    MutableCSSPropertyValueSet* style) {
  ASSERT(false); // BKTODO:
#if 0
  if (name == vspaceAttr) {
    AddHTMLLengthToStyle(style, CSSPropertyMarginTop, value);
    AddHTMLLengthToStyle(style, CSSPropertyMarginBottom, value);
  } else if (name == hspaceAttr) {
    AddHTMLLengthToStyle(style, CSSPropertyMarginLeft, value);
    AddHTMLLengthToStyle(style, CSSPropertyMarginRight, value);
  } else if (name == alignAttr) {
    if (input_type_->ShouldRespectAlignAttribute())
      ApplyAlignmentAttributeToStyle(value, style);
  } else if (name == widthAttr) {
    if (input_type_->ShouldRespectHeightAndWidthAttributes())
      AddHTMLLengthToStyle(style, CSSPropertyWidth, value);
  } else if (name == heightAttr) {
    if (input_type_->ShouldRespectHeightAndWidthAttributes())
      AddHTMLLengthToStyle(style, CSSPropertyHeight, value);
  } else if (name == borderAttr &&
             type() == InputTypeNames::image) {  // FIXME: Remove type check.
    ApplyBorderAttributeToStyle(value, style);
  } else {
    TextControlElement::CollectStyleForPresentationAttribute(name, value,
                                                             style);
  }
#endif
}

void HTMLInputElement::ParseAttribute(
    const AttributeModificationParams& params) {
  DCHECK(input_type_);
  DCHECK(input_type_view_);
  const QualifiedName& name = params.name;
  const AtomicString& value = params.new_value;

  ASSERT(false); // BKTODO:
#if 0
  if (name == nameAttr) {
    RemoveFromRadioButtonGroup();
    name_ = value;
    AddToRadioButtonGroup();
    TextControlElement::ParseAttribute(params);
  } else if (name == autocompleteAttr) {
    if (DeprecatedEqualIgnoringCase(value, "off")) {
      autocomplete_ = kOff;
    } else {
      if (value.IsEmpty())
        autocomplete_ = kUninitialized;
      else
        autocomplete_ = kOn;
    }
  } else if (name == typeAttr) {
    UpdateType();
  } else if (name == valueAttr) {
    // We only need to setChanged if the form is looking at the default value
    // right now.
    if (!HasDirtyValue()) {
      if (input_type_->GetValueMode() == ValueMode::kValue)
        non_attribute_value_ = SanitizeValue(value);
      UpdatePlaceholderVisibility();
      SetNeedsStyleRecalc(
          kSubtreeStyleChange,
          StyleChangeReasonForTracing::FromAttribute(valueAttr));
    }
    needs_to_update_view_value_ = true;
    SetNeedsValidityCheck();
    input_type_->WarnIfValueIsInvalidAndElementIsVisible(value);
    input_type_->InRangeChanged();
    input_type_view_->ValueAttributeChanged();
  } else if (name == checkedAttr) {
    // Another radio button in the same group might be checked by state
    // restore. We shouldn't call setChecked() even if this has the checked
    // attribute. So, delay the setChecked() call until
    // finishParsingChildren() is called if parsing is in progress.
    if ((!parsing_in_progress_ ||
         !GetDocument().GetFormController().HasFormStates()) &&
        !dirty_checkedness_) {
      setChecked(!value.IsNull());
      dirty_checkedness_ = false;
    }
    PseudoStateChanged(CSSSelector::kPseudoDefault);
  } else if (name == maxlengthAttr) {
    SetNeedsValidityCheck();
  } else if (name == minlengthAttr) {
    SetNeedsValidityCheck();
  } else if (name == sizeAttr) {
    unsigned size = 0;
    if (value.IsEmpty() || !ParseHTMLNonNegativeInteger(value, size) ||
        size == 0 || size > 0x7fffffffu)
      size = kDefaultSize;
    if (size_ != size) {
      size_ = size;
      if (GetLayoutObject())
        GetLayoutObject()
            ->SetNeedsLayoutAndPrefWidthsRecalcAndFullPaintInvalidation(
                LayoutInvalidationReason::kAttributeChanged);
    }
  } else if (name == altAttr) {
    input_type_view_->AltAttributeChanged();
  } else if (name == srcAttr) {
    input_type_view_->SrcAttributeChanged();
  } else if (name == usemapAttr || name == accesskeyAttr) {
    // FIXME: ignore for the moment
  } else if (name == onsearchAttr) {
    // Search field and slider attributes all just cause updateFromElement to be
    // called through style recalcing.
    SetAttributeEventListener(EventTypeNames::search,
                              CreateAttributeEventListener(this, name, value));
  } else if (name == incrementalAttr) {
    UseCounter::Count(GetDocument(), WebFeature::kIncrementalAttribute);
  } else if (name == minAttr) {
    input_type_view_->MinOrMaxAttributeChanged();
    input_type_->SanitizeValueInResponseToMinOrMaxAttributeChange();
    input_type_->InRangeChanged();
    SetNeedsValidityCheck();
    UseCounter::Count(GetDocument(), WebFeature::kMinAttribute);
  } else if (name == maxAttr) {
    input_type_view_->MinOrMaxAttributeChanged();
    input_type_->SanitizeValueInResponseToMinOrMaxAttributeChange();
    input_type_->InRangeChanged();
    SetNeedsValidityCheck();
    UseCounter::Count(GetDocument(), WebFeature::kMaxAttribute);
  } else if (name == multipleAttr) {
    input_type_view_->MultipleAttributeChanged();
    SetNeedsValidityCheck();
  } else if (name == stepAttr) {
    input_type_view_->StepAttributeChanged();
    SetNeedsValidityCheck();
    UseCounter::Count(GetDocument(), WebFeature::kStepAttribute);
  } else if (name == patternAttr) {
    SetNeedsValidityCheck();
    UseCounter::Count(GetDocument(), WebFeature::kPatternAttribute);
  } else if (name == readonlyAttr) {
    TextControlElement::ParseAttribute(params);
    input_type_view_->ReadonlyAttributeChanged();
  } else if (name == listAttr) {
    has_non_empty_list_ = !value.IsEmpty();
    if (has_non_empty_list_) {
      ResetListAttributeTargetObserver();
      ListAttributeTargetChanged();
    }
    UseCounter::Count(GetDocument(), WebFeature::kListAttribute);
  } else if (name == webkitdirectoryAttr) {
    TextControlElement::ParseAttribute(params);
    UseCounter::Count(GetDocument(), WebFeature::kPrefixedDirectoryAttribute);
  } else {
    if (name == formactionAttr)
      LogUpdateAttributeIfIsolatedWorldAndInDocument("input", params);
    TextControlElement::ParseAttribute(params);
  }
  input_type_view_->AttributeChanged();
#endif
}

void HTMLInputElement::ParserDidSetAttributes() {
  DCHECK(parsing_in_progress_);
  InitializeTypeInParsing();
}

void HTMLInputElement::FinishParsingChildren() {
  parsing_in_progress_ = false;
  DCHECK(input_type_);
  DCHECK(input_type_view_);
  TextControlElement::FinishParsingChildren();
  if (!state_restored_) {
    bool checked = hasAttribute(checkedAttr);
    if (checked)
      setChecked(checked);
    dirty_checkedness_ = false;
  }
}

bool HTMLInputElement::LayoutObjectIsNeeded(const ComputedStyle& style) const {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return input_type_->LayoutObjectIsNeeded() &&
         TextControlElement::LayoutObjectIsNeeded(style);
#endif
}

LayoutObject* HTMLInputElement::CreateLayoutObject(const ComputedStyle& style) {
  ASSERT(false); // BKTODO:
  return nullptr;
#if 0
  return input_type_view_->CreateLayoutObject(style);
#endif
}

void HTMLInputElement::AttachLayoutTree(AttachContext& context) {
  TextControlElement::AttachLayoutTree(context);
  ASSERT(false); // BKTODO:
#if 0
  if (GetLayoutObject()) {
    input_type_->OnAttachWithLayoutObject();
  }

  input_type_view_->StartResourceLoading();
  input_type_->CountUsage();
#endif
}

void HTMLInputElement::DetachLayoutTree(const AttachContext& context) {
  ASSERT(false); // BKTODO:
#if 0
  if (GetLayoutObject()) {
    input_type_->OnDetachWithLayoutObject();
  }
  TextControlElement::DetachLayoutTree(context);
  needs_to_update_view_value_ = true;
  input_type_view_->ClosePopupView();
#endif
}

String HTMLInputElement::AltText() const {
  ASSERT(false); // BKTODO:
  return String();
#if 0
  // http://www.w3.org/TR/1998/REC-html40-19980424/appendix/notes.html#altgen
  // also heavily discussed by Hixie on bugzilla
  // note this is intentionally different to HTMLImageElement::altText()
  String alt = FastGetAttribute(altAttr);
  // fall back to title attribute
  if (alt.IsNull())
    alt = FastGetAttribute(titleAttr);
  if (alt.IsNull())
    alt = FastGetAttribute(valueAttr);
  if (alt.IsNull())
    alt = GetLocale().QueryString(WebLocalizedString::kInputElementAltText);
  return alt;
#endif
}

bool HTMLInputElement::CanBeSuccessfulSubmitButton() const {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return input_type_->CanBeSuccessfulSubmitButton();
#endif
}

bool HTMLInputElement::IsActivatedSubmit() const {
  return is_activated_submit_;
}

void HTMLInputElement::SetActivatedSubmit(bool flag) {
  is_activated_submit_ = flag;
}

void HTMLInputElement::AppendToFormData(FormData& form_data) {
  ASSERT(false); // BKTODO:
#if 0
  if (input_type_->IsFormDataAppendable())
    input_type_->AppendToFormData(form_data);
#endif
}

String HTMLInputElement::ResultForDialogSubmit() {
  ASSERT(false); // BKTODO:
  return String();
#if 0
  return input_type_->ResultForDialogSubmit();
#endif
}

void HTMLInputElement::ResetImpl() {
  ASSERT(false); // BKTODO:
#if 0
  if (input_type_->GetValueMode() == ValueMode::kValue) {
    SetNonDirtyValue(DefaultValue());
    SetNeedsValidityCheck();
  } else if (input_type_->GetValueMode() == ValueMode::kFilename) {
    SetNonDirtyValue(String());
    SetNeedsValidityCheck();
  }
#endif

  setChecked(hasAttribute(checkedAttr));
  dirty_checkedness_ = false;
}

bool HTMLInputElement::IsTextField() const {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return input_type_->IsTextField();
#endif
}

bool HTMLInputElement::HasBeenPasswordField() const {
  return has_been_password_field_;
}

void HTMLInputElement::DispatchChangeEventIfNeeded() {
  ASSERT(false); // BKTODO:
#if 0
  if (isConnected() && input_type_->ShouldSendChangeEventAfterCheckedChanged())
    DispatchChangeEvent();
#endif
}

void HTMLInputElement::DispatchInputAndChangeEventIfNeeded() {
  ASSERT(false); // BKTODO:
#if 0
  if (isConnected() &&
      input_type_->ShouldSendChangeEventAfterCheckedChanged()) {
    DispatchInputEvent();
    DispatchChangeEvent();
  }
#endif
}

bool HTMLInputElement::checked() const {
  ASSERT(false); // BKTODO: input_type_->ReadingChecked();
  return is_checked_;
}

void HTMLInputElement::setChecked(bool now_checked,
                                  TextFieldEventBehavior event_behavior) {
  dirty_checkedness_ = true;
  if (checked() == now_checked)
    return;

  is_checked_ = now_checked;

  if (RadioButtonGroupScope* scope = GetRadioButtonGroupScope())
    ASSERT(false); // BKTODO: scope->UpdateCheckedState(this);
  if (LayoutObject* o = GetLayoutObject())
    o->InvalidateIfControlStateChanged(kCheckedControlState);
  SetNeedsValidityCheck();

#if 0 // BKTODO: Check if necessary.
  // Ideally we'd do this from the layout tree (matching
  // LayoutTextView), but it's not possible to do it at the moment
  // because of the way the code is structured.
  if (GetLayoutObject()) {
    if (AXObjectCache* cache =
            GetLayoutObject()->GetDocument().ExistingAXObjectCache())
      cache->CheckedStateChanged(this);
  }
#endif

  ASSERT(false); // BKTODO:
#if 0
  // Only send a change event for items in the document (avoid firing during
  // parsing) and don't send a change event for a radio button that's getting
  // unchecked to match other browsers. DOM is not a useful standard for this
  // because it says only to fire change events at "lose focus" time, which is
  // definitely wrong in practice for these types of elements.
  if (event_behavior == kDispatchInputAndChangeEvent && isConnected() &&
      input_type_->ShouldSendChangeEventAfterCheckedChanged()) {
    DispatchInputEvent();
  }
#endif

  PseudoStateChanged(CSSSelector::kPseudoChecked);
}

void HTMLInputElement::setIndeterminate(bool new_value) {
  if (indeterminate() == new_value)
    return;

  is_indeterminate_ = new_value;

  PseudoStateChanged(CSSSelector::kPseudoIndeterminate);

  if (LayoutObject* o = GetLayoutObject())
    o->InvalidateIfControlStateChanged(kCheckedControlState);
}

unsigned HTMLInputElement::size() const {
  return size_;
}

bool HTMLInputElement::SizeShouldIncludeDecoration(int& preferred_size) const {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return input_type_view_->SizeShouldIncludeDecoration(kDefaultSize,
                                                       preferred_size);
#endif
}

void HTMLInputElement::CloneNonAttributePropertiesFrom(const Element& source,
                                                       CloneChildrenFlag flag) {
  ASSERT(false); // BKTODO:
#if 0
  const HTMLInputElement& source_element = ToHTMLInputElement(source);

  non_attribute_value_ = source_element.non_attribute_value_;
  has_dirty_value_ = source_element.has_dirty_value_;
  setChecked(source_element.is_checked_);
  dirty_checkedness_ = source_element.dirty_checkedness_;
  is_indeterminate_ = source_element.is_indeterminate_;
  input_type_->CopyNonAttributeProperties(source_element);

  TextControlElement::CloneNonAttributePropertiesFrom(source, flag);

  needs_to_update_view_value_ = true;
  input_type_view_->UpdateView();
#endif
}

String HTMLInputElement::value() const {
  ASSERT(false); // BKTODO:
#if 0
  switch (input_type_->GetValueMode()) {
    case ValueMode::kFilename:
      return input_type_->ValueInFilenameValueMode();
    case ValueMode::kDefault:
      return FastGetAttribute(valueAttr);
    case ValueMode::kDefaultOn: {
      AtomicString value_string = FastGetAttribute(valueAttr);
      return value_string.IsNull() ? "on" : value_string;
    }
    case ValueMode::kValue:
      return non_attribute_value_;
  }
#endif
  NOTREACHED();
  return g_empty_string;
}

String HTMLInputElement::ValueOrDefaultLabel() const {
  String value = this->value();
  if (!value.IsNull())
    return value;
  ASSERT(false); // BKTODO:
  return value;
#if 0
  return input_type_->DefaultLabel();
#endif
}

void HTMLInputElement::SetValueForUser(const String& value) {
  // Call setValue and make it send a change event.
  setValue(value, kDispatchChangeEvent);
}

void HTMLInputElement::SetSuggestedValue(const String& value) {
  ASSERT(false); // BKTODO:
#if 0
  if (!input_type_->CanSetSuggestedValue())
    return;
  needs_to_update_view_value_ = true;
  TextControlElement::SetSuggestedValue(SanitizeValue(value));
  SetNeedsStyleRecalc(
      kSubtreeStyleChange,
      StyleChangeReasonForTracing::Create(StyleChangeReason::kControlValue));
  input_type_view_->UpdateView();
#endif
}

void HTMLInputElement::SetEditingValue(const String& value) {
  if (!GetLayoutObject() || !IsTextField())
    return;
  SetInnerEditorValue(value);
  SubtreeHasChanged();

  unsigned max = value.length();
  SetSelectionRange(max, max);
  ASSERT(false); // BKTODO: DispatchInputEvent();
}

void HTMLInputElement::SetInnerEditorValue(const String& value) {
  TextControlElement::SetInnerEditorValue(value);
  needs_to_update_view_value_ = false;
}

void HTMLInputElement::setValue(const String& value,
                                ExceptionState& exception_state,
                                TextFieldEventBehavior event_behavior) {
  ASSERT(false); // BKTODO:
#if 0
  // FIXME: Remove type check.
  if (type() == InputTypeNames::file && !value.IsEmpty()) {
    exception_state.ThrowDOMException(DOMExceptionCode::kInvalidStateError,
                                      "This input element accepts a filename, "
                                      "which may only be programmatically set "
                                      "to the empty string.");
    return;
  }
#endif
  setValue(value, event_behavior);
}

void HTMLInputElement::setValue(const String& value,
                                TextFieldEventBehavior event_behavior,
                                TextControlSetValueSelection selection) {
  ASSERT(false); // BKTODO:
#if 0
  input_type_->WarnIfValueIsInvalidAndElementIsVisible(value);
  if (!input_type_->CanSetValue(value))
    return;

  // Clear the suggested value. Use the base class version to not trigger a view
  // update.
  TextControlElement::SetSuggestedValue(String());

  // Set autofilled to false, as the value might have been set by the website.
  // If the field was autofilled, it'll be set to true from that method.
  SetAutofillState(WebAutofillState::kNotFilled);

  EventQueueScope scope;
  String sanitized_value = SanitizeValue(value);
  bool value_changed = sanitized_value != this->value();

  SetLastChangeWasNotUserEdit();
  needs_to_update_view_value_ = true;

  input_type_->SetValue(sanitized_value, value_changed, event_behavior,
                        selection);
  input_type_view_->DidSetValue(sanitized_value, value_changed);

  if (value_changed)
    NotifyFormStateChanged();
#endif
}

void HTMLInputElement::SetNonAttributeValue(const String& sanitized_value) {
  ASSERT(false); // BKTODO:
#if 0
  // This is a common code for ValueMode::kValue.
  DCHECK_EQ(input_type_->GetValueMode(), ValueMode::kValue);
  non_attribute_value_ = sanitized_value;
  has_dirty_value_ = true;
  SetNeedsValidityCheck();
  input_type_->InRangeChanged();
#endif
}

void HTMLInputElement::SetNonAttributeValueByUserEdit(
    const String& sanitized_value) {
  SetValueBeforeFirstUserEditIfNotSet();
  SetNonAttributeValue(sanitized_value);
  CheckIfValueWasReverted(sanitized_value);
}

void HTMLInputElement::SetNonDirtyValue(const String& new_value) {
  setValue(new_value);
  has_dirty_value_ = false;
}

bool HTMLInputElement::HasDirtyValue() const {
  return has_dirty_value_;
}

void HTMLInputElement::UpdateView() {
  ASSERT(false); // BKTODO: input_type_view_->UpdateView();
}

double HTMLInputElement::valueAsDate(bool& is_null) const {
  ASSERT(false); // BKTODO:
  return 0;
#if 0
  double date = input_type_->ValueAsDate();
  is_null = !std::isfinite(date);
  return date;
#endif
}

void HTMLInputElement::setValueAsDate(double value,
                                      bool is_null,
                                      ExceptionState& exception_state) {
  ASSERT(false); // BKTODO: input_type_->SetValueAsDate(value, exception_state);
}

double HTMLInputElement::valueAsNumber() const {
  ASSERT(false); // BKTODO:
  return 0;
#if 0
  return input_type_->ValueAsDouble();
#endif
}

void HTMLInputElement::setValueAsNumber(double new_value,
                                        ExceptionState& exception_state,
                                        TextFieldEventBehavior event_behavior) {
  // http://www.whatwg.org/specs/web-apps/current-work/multipage/common-input-element-attributes.html#dom-input-valueasnumber
  // On setting, if the new value is infinite, then throw a TypeError exception.
  if (std::isinf(new_value)) {
    exception_state.ThrowTypeError(
        ExceptionMessages::NotAFiniteNumber(new_value));
    return;
  }
  ASSERT(false); // BKTODO: input_type_->SetValueAsDouble(new_value, event_behavior, exception_state);
}

void HTMLInputElement::SetValueFromRenderer(const String& value) {
  ASSERT(false); // BKTODO:
#if 0
  // File upload controls will never use this.
  DCHECK_NE(type(), InputTypeNames::file);

  // Clear the suggested value. Use the base class version to not trigger a view
  // update.
  TextControlElement::SetSuggestedValue(String());

  // Renderer and our event handler are responsible for sanitizing values.
  DCHECK(value == input_type_->SanitizeUserInputValue(value) ||
         input_type_->SanitizeUserInputValue(value).IsEmpty());

  DCHECK(!value.IsNull());
  SetValueBeforeFirstUserEditIfNotSet();
  non_attribute_value_ = value;
  has_dirty_value_ = true;
  needs_to_update_view_value_ = false;
  CheckIfValueWasReverted(value);

  // Input event is fired by the Node::defaultEventHandler for editable
  // controls.
  if (!IsTextField())
    DispatchInputEvent();
  NotifyFormStateChanged();

  SetNeedsValidityCheck();

  // Clear autofill flag (and yellow background) on user edit.
  SetAutofillState(WebAutofillState::kNotFilled);
#endif
}

EventDispatchHandlingState* HTMLInputElement::PreDispatchEventHandler(
    Event& event) {
  ASSERT(false); // BKTODO:
  return nullptr;
#if 0
  if (event.type() == EventTypeNames::textInput &&
      input_type_view_->ShouldSubmitImplicitly(event)) {
    event.stopPropagation();
    return nullptr;
  }
  if (event.type() != EventTypeNames::click)
    return nullptr;
  if (!event.IsMouseEvent() ||
      ToMouseEvent(event).button() !=
          static_cast<short>(WebPointerProperties::Button::kLeft))
    return nullptr;
  return input_type_view_->WillDispatchClick();
#endif
}

void HTMLInputElement::PostDispatchEventHandler(
    Event& event,
    EventDispatchHandlingState* state) {
  if (!state)
    return;
  ASSERT(false); // BKTODO:
#if 0
  input_type_view_->DidDispatchClick(event,
                                     *static_cast<ClickHandlingState*>(state));
#endif
}

void HTMLInputElement::DefaultEventHandler(Event& evt) {
  ASSERT(false); // BKTODO:
#if 0
  if (evt.IsMouseEvent() && evt.type() == EventTypeNames::click &&
      ToMouseEvent(evt).button() ==
          static_cast<short>(WebPointerProperties::Button::kLeft)) {
    input_type_view_->HandleClickEvent(ToMouseEvent(evt));
    if (evt.DefaultHandled())
      return;
  }

  if (evt.IsKeyboardEvent() && evt.type() == EventTypeNames::keydown) {
    input_type_view_->HandleKeydownEvent(ToKeyboardEvent(evt));
    if (evt.DefaultHandled())
      return;
  }

  // Call the base event handler before any of our own event handling for almost
  // all events in text fields.  Makes editing keyboard handling take precedence
  // over the keydown and keypress handling in this function.
  bool call_base_class_early =
      IsTextField() && (evt.type() == EventTypeNames::keydown ||
                        evt.type() == EventTypeNames::keypress);
  if (call_base_class_early) {
    TextControlElement::DefaultEventHandler(evt);
    if (evt.DefaultHandled())
      return;
  }

  // DOMActivate events cause the input to be "activated" - in the case of image
  // and submit inputs, this means actually submitting the form. For reset
  // inputs, the form is reset. These events are sent when the user clicks on
  // the element, or presses enter while it is the active element. JavaScript
  // code wishing to activate the element must dispatch a DOMActivate event - a
  // click event will not do the job.
  if (evt.type() == EventTypeNames::DOMActivate) {
    input_type_view_->HandleDOMActivateEvent(evt);
    if (evt.DefaultHandled())
      return;
  }

  // Use key press event here since sending simulated mouse events
  // on key down blocks the proper sending of the key press event.
  if (evt.IsKeyboardEvent() && evt.type() == EventTypeNames::keypress) {
    input_type_view_->HandleKeypressEvent(ToKeyboardEvent(evt));
    if (evt.DefaultHandled())
      return;
  }

  if (evt.IsKeyboardEvent() && evt.type() == EventTypeNames::keyup) {
    input_type_view_->HandleKeyupEvent(ToKeyboardEvent(evt));
    if (evt.DefaultHandled())
      return;
  }

  if (input_type_view_->ShouldSubmitImplicitly(evt)) {
    // FIXME: Remove type check.
    if (type() == InputTypeNames::search) {
      GetDocument()
          .GetTaskRunner(TaskType::kUserInteraction)
          ->PostTask(FROM_HERE, WTF::Bind(&HTMLInputElement::OnSearch,
                                          WrapPersistent(this)));
    }
    // Form submission finishes editing, just as loss of focus does.
    // If there was a change, send the event now.
    DispatchFormControlChangeEvent();

    HTMLFormElement* form_for_submission =
        input_type_view_->FormForSubmission();
    // Form may never have been present, or may have been destroyed by code
    // responding to the change event.
    if (form_for_submission) {
      form_for_submission->SubmitImplicitly(evt,
                                            CanTriggerImplicitSubmission());
    }
    evt.SetDefaultHandled();
    return;
  }

  if (evt.IsBeforeTextInsertedEvent()) {
    input_type_view_->HandleBeforeTextInsertedEvent(
        static_cast<BeforeTextInsertedEvent&>(evt));
  }

  if (evt.IsMouseEvent() && evt.type() == EventTypeNames::mousedown) {
    input_type_view_->HandleMouseDownEvent(ToMouseEvent(evt));
    if (evt.DefaultHandled())
      return;
  }

  input_type_view_->ForwardEvent(evt);

  if (!call_base_class_early && !evt.DefaultHandled())
    TextControlElement::DefaultEventHandler(evt);
#endif
}

void HTMLInputElement::CreateShadowSubtree() {
  ASSERT(false); // BKTODO: input_type_view_->CreateShadowSubtree();
}

bool HTMLInputElement::HasActivationBehavior() const {
  return true;
}

bool HTMLInputElement::WillRespondToMouseClickEvents() {
  // FIXME: Consider implementing willRespondToMouseClickEvents() in InputType
  // if more accurate results are necessary.
  if (!IsDisabledFormControl())
    return true;

  return TextControlElement::WillRespondToMouseClickEvents();
}

bool HTMLInputElement::IsURLAttribute(const Attribute& attribute) const {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return attribute.GetName() == srcAttr ||
         attribute.GetName() == formactionAttr ||
         TextControlElement::IsURLAttribute(attribute);
#endif
}

bool HTMLInputElement::HasLegalLinkAttribute(const QualifiedName& name) const {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return input_type_->HasLegalLinkAttribute(name) ||
         TextControlElement::HasLegalLinkAttribute(name);
#endif
}

const QualifiedName& HTMLInputElement::SubResourceAttributeName() const {
  ASSERT(false); // BKTODO:
  exit(0);
#if 0
  return input_type_->SubResourceAttributeName();
#endif
}

const AtomicString& HTMLInputElement::DefaultValue() const {
  return FastGetAttribute(valueAttr);
}

static inline bool IsRFC2616TokenCharacter(UChar ch) {
  return IsASCII(ch) && ch > ' ' && ch != '"' && ch != '(' && ch != ')' &&
         ch != ',' && ch != '/' && (ch < ':' || ch > '@') &&
         (ch < '[' || ch > ']') && ch != '{' && ch != '}' && ch != 0x7f;
}

static bool IsValidMIMEType(const String& type) {
  size_t slash_position = type.find('/');
  if (slash_position == kNotFound || !slash_position ||
      slash_position == type.length() - 1)
    return false;
  for (wtf_size_t i = 0; i < type.length(); ++i) {
    if (!IsRFC2616TokenCharacter(type[i]) && i != slash_position)
      return false;
  }
  return true;
}

static bool IsValidFileExtension(const String& type) {
  if (type.length() < 2)
    return false;
  return type[0] == '.';
}

static Vector<String> ParseAcceptAttribute(const String& accept_string,
                                           bool (*predicate)(const String&)) {
  Vector<String> types;
  if (accept_string.IsEmpty())
    return types;

  Vector<String> split_types;
  accept_string.Split(',', false, split_types);
  for (const String& split_type : split_types) {
    String trimmed_type = StripLeadingAndTrailingHTMLSpaces(split_type);
    if (trimmed_type.IsEmpty())
      continue;
    if (!predicate(trimmed_type))
      continue;
    types.push_back(trimmed_type.DeprecatedLower());
  }

  return types;
}

Vector<String> HTMLInputElement::AcceptMIMETypes() const {
  return ParseAcceptAttribute(FastGetAttribute(acceptAttr), IsValidMIMEType);
}

Vector<String> HTMLInputElement::AcceptFileExtensions() const {
  return ParseAcceptAttribute(FastGetAttribute(acceptAttr),
                              IsValidFileExtension);
}

const AtomicString& HTMLInputElement::Alt() const {
  ASSERT(false); // BKTODO:
  return g_null_atom;
#if 0
  return FastGetAttribute(altAttr);
#endif
}

bool HTMLInputElement::Multiple() const {
  return FastHasAttribute(multipleAttr);
}

void HTMLInputElement::setSize(unsigned size, ExceptionState& exception_state) {
  if (size == 0) {
    exception_state.ThrowDOMException(
        DOMExceptionCode::kIndexSizeError,
        "The value provided is 0, which is an invalid size.");
  } else {
    ASSERT(false); // BKTODO:
#if 0
    SetUnsignedIntegralAttribute(sizeAttr, size ? size : kDefaultSize,
                                 kDefaultSize);
#endif
  }
}

GURL HTMLInputElement::Src() const {
  return GetDocument().CompleteURL(FastGetAttribute(srcAttr));
}

FileList* HTMLInputElement::files() const {
  ASSERT(false); // BKTODO:
  return nullptr;
#if 0
  return input_type_->Files();
#endif
}

void HTMLInputElement::setFiles(FileList* files) {
  ASSERT(false); // BKTODO: input_type_->SetFiles(files);
}

bool HTMLInputElement::ReceiveDroppedFiles(const DragData* drag_data) {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return input_type_->ReceiveDroppedFiles(drag_data);
#endif
}

String HTMLInputElement::DroppedFileSystemId() {
  ASSERT(false); // BKTODO:
  return String();
#if 0
  return input_type_->DroppedFileSystemId();
#endif
}

bool HTMLInputElement::CanReceiveDroppedFiles() const {
  return can_receive_dropped_files_;
}

void HTMLInputElement::SetCanReceiveDroppedFiles(
    bool can_receive_dropped_files) {
  if (!!can_receive_dropped_files_ == can_receive_dropped_files)
    return;
  can_receive_dropped_files_ = can_receive_dropped_files;
  if (GetLayoutObject())
    GetLayoutObject()->UpdateFromElement();
}

String HTMLInputElement::SanitizeValue(const String& proposed_value) const {
  ASSERT(false); // BKTODO:
  return proposed_value;
#if 0
  return input_type_->SanitizeValue(proposed_value);
#endif
}

String HTMLInputElement::LocalizeValue(const String& proposed_value) const {
  if (proposed_value.IsNull())
    return proposed_value;
  ASSERT(false); // BKTODO:
  return proposed_value;
#if 0
  return input_type_->LocalizeValue(proposed_value);
#endif
}

bool HTMLInputElement::IsInRange() const {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return willValidate() && input_type_->IsInRange(value());
#endif
}

bool HTMLInputElement::IsOutOfRange() const {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return willValidate() && input_type_->IsOutOfRange(value());
#endif
}

bool HTMLInputElement::IsRequiredFormControl() const {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return input_type_->SupportsRequired() && IsRequired();
#endif
}

bool HTMLInputElement::MatchesReadOnlyPseudoClass() const {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return input_type_->SupportsReadOnly() && IsReadOnly();
#endif
}

bool HTMLInputElement::MatchesReadWritePseudoClass() const {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return input_type_->SupportsReadOnly() && !IsReadOnly();
#endif
}

void HTMLInputElement::OnSearch() {
  ASSERT(false); // BKTODO: input_type_->DispatchSearchEvent();
}

void HTMLInputElement::UpdateClearButtonVisibility() {
  ASSERT(false); // BKTODO: input_type_view_->UpdateClearButtonVisibility();
}

void HTMLInputElement::WillChangeForm() {
  if (input_type_)
    RemoveFromRadioButtonGroup();
  TextControlElement::WillChangeForm();
}

void HTMLInputElement::DidChangeForm() {
  TextControlElement::DidChangeForm();
  if (input_type_)
    AddToRadioButtonGroup();
}

Node::InsertionNotificationRequest HTMLInputElement::InsertedInto(
    ContainerNode& insertion_point) {
  TextControlElement::InsertedInto(insertion_point);
  if (insertion_point.isConnected() && !Form())
    AddToRadioButtonGroup();
  ResetListAttributeTargetObserver();
  return kInsertionShouldCallDidNotifySubtreeInsertions;
}

void HTMLInputElement::RemovedFrom(ContainerNode& insertion_point) {
  ASSERT(false); // BKTODO: input_type_view_->ClosePopupView();
  if (insertion_point.isConnected() && !Form())
    RemoveFromRadioButtonGroup();
  TextControlElement::RemovedFrom(insertion_point);
  DCHECK(!isConnected());
  ResetListAttributeTargetObserver();
}

#if 0 // BKTODO:
void HTMLInputElement::DidMoveToNewDocument(Document& old_document) {
  if (ImageLoader())
    ImageLoader()->ElementDidMoveToNewDocument();

  // FIXME: Remove type check.
  if (type() == InputTypeNames::radio)
    GetTreeScope().GetRadioButtonGroupScope().RemoveButton(this);

  TextControlElement::DidMoveToNewDocument(old_document);
}
#endif

bool HTMLInputElement::RecalcWillValidate() const {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return input_type_->SupportsValidation() &&
         TextControlElement::RecalcWillValidate();
#endif
}

void HTMLInputElement::RequiredAttributeChanged() {
  TextControlElement::RequiredAttributeChanged();
  ASSERT(false); // BKTODO:
#if 0
  if (RadioButtonGroupScope* scope = GetRadioButtonGroupScope())
    scope->RequiredAttributeChanged(this);
  input_type_view_->RequiredAttributeChanged();
#endif
}

void HTMLInputElement::DisabledAttributeChanged() {
  TextControlElement::DisabledAttributeChanged();
  ASSERT(false); // BKTODO: input_type_view_->DisabledAttributeChanged();
}

void HTMLInputElement::SelectColorInColorChooser(const Color& color) {
  ASSERT(false); // BKTODO:
#if 0
  if (ColorChooserClient* client = input_type_->GetColorChooserClient())
    client->DidChooseColor(color);
#endif
}

void HTMLInputElement::EndColorChooser() {
  ASSERT(false); // BKTODO:
#if 0
  if (ColorChooserClient* client = input_type_->GetColorChooserClient())
    client->DidEndChooser();
#endif
}

HTMLElement* HTMLInputElement::list() const {
  return DataList();
}

HTMLDataListElement* HTMLInputElement::DataList() const {
  if (!has_non_empty_list_)
    return nullptr;

  ASSERT(false); // BKTODO:
  return nullptr;
#if 0
  if (!input_type_->ShouldRespectListAttribute())
    return nullptr;

  return ToHTMLDataListElementOrNull(
      GetTreeScope().getElementById(FastGetAttribute(listAttr)));
#endif
}

bool HTMLInputElement::HasValidDataListOptions() const {
  HTMLDataListElement* data_list = DataList();
  if (!data_list)
    return false;
  HTMLDataListOptionsCollection* options = data_list->options();
  ASSERT(false); // BKTODO:
#if 0
  for (unsigned i = 0; HTMLOptionElement* option = options->Item(i); ++i) {
    if (!option->value().IsEmpty() && !option->IsDisabledFormControl() &&
        IsValidValue(option->value()))
      return true;
  }
#endif
  return false;
}

#if 0 // BKTODO:
HeapVector<Member<HTMLOptionElement>>
HTMLInputElement::FilteredDataListOptions() const {
  HeapVector<Member<HTMLOptionElement>> filtered;
  HTMLDataListElement* data_list = DataList();
  if (!data_list)
    return filtered;

  String value = InnerEditorValue();
  if (Multiple() && type() == InputTypeNames::email) {
    Vector<String> emails;
    value.Split(',', true, emails);
    if (!emails.IsEmpty())
      value = emails.back().StripWhiteSpace();
  }

  HTMLDataListOptionsCollection* options = data_list->options();
  filtered.ReserveCapacity(options->length());
  value = value.FoldCase();
  for (unsigned i = 0; i < options->length(); ++i) {
    HTMLOptionElement* option = options->Item(i);
    DCHECK(option);
    if (!value.IsEmpty()) {
      // Firefox shows OPTIONs with matched labels, Edge shows OPTIONs
      // with matches values. We show both.
      if (option->value().FoldCase().Find(value) == kNotFound &&
          option->label().FoldCase().Find(value) == kNotFound)
        continue;
    }
    // TODO(tkent): Should allow invalid strings. crbug.com/607097.
    if (option->value().IsEmpty() || option->IsDisabledFormControl() ||
        !IsValidValue(option->value()))
      continue;
    filtered.push_back(option);
  }
  return filtered;
}
#endif

void HTMLInputElement::SetListAttributeTargetObserver(
    ListAttributeTargetObserver* new_observer) {
  ASSERT(false); // BKTODO:
#if 0
  if (list_attribute_target_observer_)
    list_attribute_target_observer_->Unregister();
  list_attribute_target_observer_ = new_observer;
#endif
}

void HTMLInputElement::ResetListAttributeTargetObserver() {
  ASSERT(false); // BKTODO:
#if 0
  const AtomicString& value = FastGetAttribute(listAttr);
  if (!value.IsNull() && isConnected()) {
    SetListAttributeTargetObserver(
        ListAttributeTargetObserver::Create(value, this));
  } else {
    SetListAttributeTargetObserver(nullptr);
  }
#endif
}

void HTMLInputElement::ListAttributeTargetChanged() {
  ASSERT(false); // BKTODO: input_type_view_->ListAttributeTargetChanged();
}

bool HTMLInputElement::IsSteppable() const {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return input_type_->IsSteppable();
#endif
}

bool HTMLInputElement::IsTextButton() const {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return input_type_->IsTextButton();
#endif
}

bool HTMLInputElement::IsEnumeratable() const {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return input_type_->IsEnumeratable();
#endif
}

bool HTMLInputElement::SupportLabels() const {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return input_type_->IsInteractiveContent();
#endif
}

bool HTMLInputElement::MatchesDefaultPseudoClass() const {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return input_type_->MatchesDefaultPseudoClass();
#endif
}

bool HTMLInputElement::ShouldAppearChecked() const {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return checked() && input_type_->IsCheckable();
#endif
}

void HTMLInputElement::SetPlaceholderVisibility(bool visible) {
  is_placeholder_visible_ = visible;
}

bool HTMLInputElement::SupportsPlaceholder() const {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return input_type_->SupportsPlaceholder();
#endif
}

void HTMLInputElement::UpdatePlaceholderText() {
  ASSERT(false); // BKTODO: return input_type_view_->UpdatePlaceholderText();
}

String HTMLInputElement::GetPlaceholderValue() const {
  return !SuggestedValue().IsEmpty() ? SuggestedValue() : StrippedPlaceholder();
}

String HTMLInputElement::DefaultToolTip() const {
  ASSERT(false); // BKTODO:
  return String();
#if 0
  return input_type_->DefaultToolTip(*input_type_view_);
#endif
}

bool HTMLInputElement::ShouldAppearIndeterminate() const {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return input_type_->ShouldAppearIndeterminate();
#endif
}

bool HTMLInputElement::IsInRequiredRadioButtonGroup() {
  ASSERT(false); // BKTODO:
#if 0
  // TODO(tkent): Remove type check.
  DCHECK_EQ(type(), InputTypeNames::radio);
  if (RadioButtonGroupScope* scope = GetRadioButtonGroupScope())
    return scope->IsInRequiredGroup(this);
#endif
  return false;
}

HTMLInputElement* HTMLInputElement::CheckedRadioButtonForGroup() {
  if (checked())
    return this;
  if (RadioButtonGroupScope* scope = GetRadioButtonGroupScope())
    ASSERT(false); // BKTODO: return scope->CheckedButtonForGroup(GetName());
  return nullptr;
}

RadioButtonGroupScope* HTMLInputElement::GetRadioButtonGroupScope() const {
  ASSERT(false); // BKTODO:
#if 0
  // FIXME: Remove type check.
  if (type() != InputTypeNames::radio)
    return nullptr;
  if (HTMLFormElement* form_element = Form())
    return &form_element->GetRadioButtonGroupScope();
  if (isConnected())
    return &GetTreeScope().GetRadioButtonGroupScope();
#endif
  return nullptr;
}

unsigned HTMLInputElement::SizeOfRadioGroup() const {
  RadioButtonGroupScope* scope = GetRadioButtonGroupScope();
  if (!scope)
    return 0;
  ASSERT(false); // BKTODO:
  return 0;
#if 0
  return scope->GroupSizeFor(this);
#endif
}

inline void HTMLInputElement::AddToRadioButtonGroup() {
  if (RadioButtonGroupScope* scope = GetRadioButtonGroupScope())
    ASSERT(false); // BKTODO: scope->AddButton(this);
}

inline void HTMLInputElement::RemoveFromRadioButtonGroup() {
  if (RadioButtonGroupScope* scope = GetRadioButtonGroupScope())
    ASSERT(false); // BKTODO: scope->RemoveButton(this);
}

unsigned HTMLInputElement::height() const {
  ASSERT(false); // BKTODO:
  return 0;
#if 0
  return input_type_->Height();
#endif
}

unsigned HTMLInputElement::width() const {
  ASSERT(false); // BKTODO:
  return 0;
#if 0
  return input_type_->Width();
#endif
}

void HTMLInputElement::setHeight(unsigned height) {
  ASSERT(false); // BKTODO: SetUnsignedIntegralAttribute(heightAttr, height);
}

void HTMLInputElement::setWidth(unsigned width) {
  ASSERT(false); // BKTODO: SetUnsignedIntegralAttribute(widthAttr, width);
}

#if 0 // BKTODO:
ListAttributeTargetObserver* ListAttributeTargetObserver::Create(
    const AtomicString& id,
    HTMLInputElement* element) {
  return new ListAttributeTargetObserver(id, element);
}

ListAttributeTargetObserver::ListAttributeTargetObserver(
    const AtomicString& id,
    HTMLInputElement* element)
    : IdTargetObserver(element->GetTreeScope().GetIdTargetObserverRegistry(),
                       id),
      element_(element) {}

void ListAttributeTargetObserver::Trace(blink::Visitor* visitor) {
  visitor->Trace(element_);
  IdTargetObserver::Trace(visitor);
}

void ListAttributeTargetObserver::IdTargetChanged() {
  element_->ListAttributeTargetChanged();
}
#endif

void HTMLInputElement::setRangeText(const String& replacement,
                                    ExceptionState& exception_state) {
  ASSERT(false); // BKTODO:
#if 0
  if (!input_type_->SupportsSelectionAPI()) {
    exception_state.ThrowDOMException(DOMExceptionCode::kInvalidStateError,
                                      "The input element's type ('" +
                                          input_type_->FormControlType() +
                                          "') does not support selection.");
    return;
  }
#endif

  TextControlElement::setRangeText(replacement, exception_state);
}

void HTMLInputElement::setRangeText(const String& replacement,
                                    unsigned start,
                                    unsigned end,
                                    const String& selection_mode,
                                    ExceptionState& exception_state) {
  ASSERT(false); // BKTODO:
#if 0
  if (!input_type_->SupportsSelectionAPI()) {
    exception_state.ThrowDOMException(DOMExceptionCode::kInvalidStateError,
                                      "The input element's type ('" +
                                          input_type_->FormControlType() +
                                          "') does not support selection.");
    return;
  }
#endif

  TextControlElement::setRangeText(replacement, start, end, selection_mode,
                                   exception_state);
}

bool HTMLInputElement::SetupDateTimeChooserParameters(
    DateTimeChooserParameters& parameters) {
  if (!GetDocument().View())
    return false;

  ASSERT(false); // BKTODO:
#if 0
  parameters.type = type();
  parameters.minimum = Minimum();
  parameters.maximum = Maximum();
  parameters.required = IsRequired();
  if (!RuntimeEnabledFeatures::LangAttributeAwareFormControlUIEnabled()) {
    parameters.locale = DefaultLanguage();
  } else {
    AtomicString computed_locale = ComputeInheritedLanguage();
    parameters.locale =
        computed_locale.IsEmpty() ? DefaultLanguage() : computed_locale;
  }

  StepRange step_range = CreateStepRange(kRejectAny);
  if (step_range.HasStep()) {
    parameters.step = step_range.Step().ToDouble();
    parameters.step_base = step_range.StepBase().ToDouble();
  } else {
    parameters.step = 1.0;
    parameters.step_base = 0;
  }

  parameters.anchor_rect_in_screen =
      GetDocument().View()->FrameToScreen(PixelSnappedBoundingBox());
  parameters.double_value = input_type_->ValueAsDouble();
  parameters.is_anchor_element_rtl =
      input_type_view_->ComputedTextDirection() == TextDirection::kRtl;
  if (HTMLDataListElement* data_list = DataList()) {
    HTMLDataListOptionsCollection* options = data_list->options();
    for (unsigned i = 0; HTMLOptionElement* option = options->Item(i); ++i) {
      if (option->value().IsEmpty() || option->IsDisabledFormControl() ||
          !IsValidValue(option->value()))
        continue;
      DateTimeSuggestion suggestion;
      suggestion.value =
          input_type_->ParseToNumber(option->value(), Decimal::Nan())
              .ToDouble();
      if (std::isnan(suggestion.value))
        continue;
      suggestion.localized_value = LocalizeValue(option->value());
      suggestion.label =
          option->value() == option->label() ? String() : option->label();
      parameters.suggestions.push_back(suggestion);
    }
  }
#endif
  return true;
}

bool HTMLInputElement::SupportsInputModeAttribute() const {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return input_type_->SupportsInputModeAttribute();
#endif
}

void HTMLInputElement::SetShouldRevealPassword(bool value) {
  if (!!should_reveal_password_ == value)
    return;
  should_reveal_password_ = value;
  LazyReattachIfAttached();
}

bool HTMLInputElement::IsInteractiveContent() const {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return input_type_->IsInteractiveContent();
#endif
}

bool HTMLInputElement::SupportsAutofocus() const {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return input_type_->IsInteractiveContent();
#endif
}

scoped_refptr<ComputedStyle> HTMLInputElement::CustomStyleForLayoutObject() {
  ASSERT(false); // BKTODO:
  return nullptr;
#if 0
  return input_type_view_->CustomStyleForLayoutObject(
      OriginalStyleForLayoutObject());
#endif
}

void HTMLInputElement::DidRecalcStyle(StyleRecalcChange change) {
  TextControlElement::DidRecalcStyle(change);
  if (change != kReattach)
    return;
  ComputedStyle* style = GetNonAttachedStyle();
  if (style && style->Display() != EDisplay::kNone)
    ASSERT(false); // BKTODO: input_type_view_->StartResourceLoading();
}

void HTMLInputElement::DidNotifySubtreeInsertionsToDocument() {
  ListAttributeTargetChanged();
}

#if 0 // BKTODO: Check if necessary.
AXObject* HTMLInputElement::PopupRootAXObject() {
  return input_type_view_->PopupRootAXObject();
}
#endif

void HTMLInputElement::EnsureFallbackContent() {
  ASSERT(false); // BKTODO: input_type_view_->EnsureFallbackContent();
}

void HTMLInputElement::EnsurePrimaryContent() {
  ASSERT(false); // BKTODO: input_type_view_->EnsurePrimaryContent();
}

bool HTMLInputElement::HasFallbackContent() const {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return input_type_view_->HasFallbackContent();
#endif
}

void HTMLInputElement::SetFilesFromPaths(const Vector<String>& paths) {
  ASSERT(false); // BKTODO: return input_type_->SetFilesFromPaths(paths);
}

void HTMLInputElement::ChildrenChanged(const ChildrenChange& change) {
  // Some input types only need shadow roots to hide any children that may
  // have been appended by script. For such types, shadow roots are lazily
  // created when children are added for the first time.
  EnsureUserAgentShadowRoot();
  ContainerNode::ChildrenChanged(change);
}

}  // namespace blink
