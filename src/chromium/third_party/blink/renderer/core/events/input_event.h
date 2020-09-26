// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: input_event.h
// Description: InputEvent Class
//      Author: Ziming Li
//     Created: 2020-09-25
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_EVENTS_INPUT_EVENT_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_EVENTS_INPUT_EVENT_H_

// BKTODO: #include "third_party/blink/renderer/core/clipboard/data_transfer.h"
#include "third_party/blink/renderer/core/dom/range.h"
#include "third_party/blink/renderer/core/dom/static_range.h"
#include "third_party/blink/renderer/core/events/input_event_init.h"
#include "third_party/blink/renderer/core/events/ui_event.h"

namespace blink {

class InputEvent final : public UIEvent {
  DEFINE_WRAPPERTYPEINFO();

 public:
  static InputEvent* Create(const AtomicString& type,
                            const InputEventInit& initializer) {
    return new InputEvent(type, initializer);
  }

  // https://w3c.github.io/input-events/#h-interface-inputevent-attributes
  enum class InputType {
    kNone,
    // Insertion.
    kInsertText,
    kInsertLineBreak,
    kInsertParagraph,
    kInsertOrderedList,
    kInsertUnorderedList,
    kInsertHorizontalRule,
    kInsertFromPaste,
    kInsertFromDrop,
    kInsertFromYank,
    kInsertTranspose,
    kInsertReplacementText,
    kInsertCompositionText,
    // Deletion.
    kDeleteWordBackward,
    kDeleteWordForward,
    kDeleteSoftLineBackward,
    kDeleteSoftLineForward,
    kDeleteHardLineBackward,
    kDeleteHardLineForward,
    kDeleteContentBackward,
    kDeleteContentForward,
    kDeleteByCut,
    kDeleteByDrag,
    // History.
    kHistoryUndo,
    kHistoryRedo,
    // Formatting.
    kFormatBold,
    kFormatItalic,
    kFormatUnderline,
    kFormatStrikeThrough,
    kFormatSuperscript,
    kFormatSubscript,
    kFormatJustifyCenter,
    kFormatJustifyFull,
    kFormatJustifyRight,
    kFormatJustifyLeft,
    kFormatIndent,
    kFormatOutdent,
    kFormatRemove,
    kFormatSetBlockTextDirection,

    // Add new input types immediately above this line.
    kNumberOfInputTypes,
  };

  enum EventCancelable : bool {
    kNotCancelable = false,
    kIsCancelable = true,
  };

  enum EventIsComposing : bool {
    kNotComposing = false,
    kIsComposing = true,
  };

  static InputEvent* CreateBeforeInput(InputType,
                                       const String& data,
                                       EventCancelable,
                                       EventIsComposing,
                                       const StaticRangeVector*);
#if 0 // BKTODO:
  static InputEvent* CreateBeforeInput(InputType,
                                       DataTransfer*,
                                       EventCancelable,
                                       EventIsComposing,
                                       const StaticRangeVector*);
#endif
  static InputEvent* CreateInput(InputType,
                                 const String& data,
                                 EventIsComposing,
                                 const StaticRangeVector*);

  String inputType() const;
  const String& data() const { return data_; }
#if 0 // BKTODO:
  DataTransfer* dataTransfer() const { return data_transfer_.Get(); }
#endif
  bool isComposing() const { return is_composing_; }
  // Returns a copy of target ranges during event dispatch, and returns an empty
  // vector after dispatch.
  StaticRangeVector getTargetRanges() const;

  bool IsInputEvent() const override;

  DispatchEventResult DispatchEvent(EventDispatcher&) override;

 private:
  InputEvent(const AtomicString&, const InputEventInit&);

  InputType input_type_;
  String data_;
#if 0 // BKTODO:
  Member<DataTransfer> data_transfer_;
#endif
  bool is_composing_;

#if 0 // BKTODO:
  // We have to stored |Range| internally and only expose |StaticRange|, please
  // see comments in |dispatchEvent()|.
  RangeVector ranges_;
#endif
};

DEFINE_EVENT_TYPE_CASTS(InputEvent);

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_EVENTS_INPUT_EVENT_H_
