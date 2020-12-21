// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: scripted_animation_controller.h
// Description: ScriptedAnimationController Class
//      Author: Ziming Li
//     Created: 2020-12-17
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2011 Google Inc. All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 *  THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_DOM_SCRIPTED_ANIMATION_CONTROLLER_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_DOM_SCRIPTED_ANIMATION_CONTROLLER_H_

#include "third_party/blink/renderer/core/core_export.h"
// BKTODO: #include "third_party/blink/renderer/core/dom/frame_request_callback_collection.h"
#include "third_party/blink/renderer/platform/bindings/name_client.h"
#include "third_party/blink/renderer/platform/bindings/trace_wrapper_member.h"
#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/wtf/text/atomic_string.h"
#include "third_party/blink/renderer/platform/wtf/text/string_impl.h"
#include "third_party/blink/renderer/platform/wtf/vector.h"

namespace blink {

class Document;
class Event;
class EventTarget;
class MediaQueryListListener;

class CORE_EXPORT ScriptedAnimationController
    : public NameClient {
 public:
  using EventData = std::pair<Member<const EventTarget>, const StringImpl *>;

  static std::unique_ptr<ScriptedAnimationController> Create(Document* document) {
    return base::WrapUnique(new ScriptedAnimationController(document));
  }
  virtual ~ScriptedAnimationController() = default;

  void Trace(blink::Visitor*);
  const char* NameInHeapSnapshot() const override {
    return "ScriptedAnimationController";
  }
  void ClearDocumentPointer() { document_ = nullptr; }

  // Animation frame callbacks are used for requestAnimationFrame().
  typedef int CallbackId;
#if 0 // BKTODO:
  CallbackId RegisterCallback(FrameRequestCallbackCollection::FrameCallback*);
#endif
  void CancelCallback(CallbackId);
  // Returns true if any callback is currently registered.
  bool HasCallback() const;

  // Animation frame events are used for resize events, scroll events, etc.
  void EnqueueEvent(Event*);
  void EnqueuePerFrameEvent(Event*);

#if 0 // BKTODO:
  // Animation frame tasks are used for Fullscreen.
  void EnqueueTask(base::OnceClosure);
#endif

  // Used for the MediaQueryList change event.
  void EnqueueMediaQueryChangeListeners(
      HeapVector<Member<MediaQueryListListener>>&);

  // Invokes callbacks, dispatches events, etc. The order is defined by HTML:
  // https://html.spec.whatwg.org/multipage/webappapis.html#event-loop-processing-model
  void ServiceScriptedAnimations(base::TimeTicks monotonic_time_now);

  void Pause();
  void Unpause();

  void DispatchEventsAndCallbacksForPrinting();

  bool CurrentFrameHadRAF() const { return current_frame_had_raf_; }
  bool NextFrameHasPendingRAF() const { return next_frame_has_pending_raf_; }

 private:
  explicit ScriptedAnimationController(Document*);

  void ScheduleAnimationIfNeeded();

  void RunTasks();
  void DispatchEvents(
      const AtomicString& event_interface_filter = AtomicString());
  void ExecuteCallbacks(base::TimeTicks monotonic_time_now);
  void CallMediaQueryListListeners();

  bool HasScheduledItems() const;

  Member<Document> document_;
#if 0 // BKTODO:
  FrameRequestCallbackCollection callback_collection_;
#endif
  int suspend_count_;
#if 0 // BKTODO:
  Vector<base::OnceClosure> task_queue_;
#endif
  HeapVector<Member<Event>> event_queue_;
  HeapListHashSet<EventData> per_frame_events_;
  using MediaQueryListListeners =
      HeapListHashSet<Member<MediaQueryListListener>>;
  MediaQueryListListeners media_query_list_listeners_;

  // Used for animation metrics; see cc::CompositorTimingHistory::DidDraw.
  bool current_frame_had_raf_;
  bool next_frame_has_pending_raf_;
};

}  // namespace blink

namespace std {

template<>
struct hash<blink::ScriptedAnimationController::EventData> {
  std::size_t operator()(const blink::ScriptedAnimationController::EventData &d) const noexcept {
    return WTF::HashInts(reinterpret_cast<unsigned>(d.first.Get()), reinterpret_cast<unsigned>(d.second));
  }
};

}

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_DOM_SCRIPTED_ANIMATION_CONTROLLER_H_
