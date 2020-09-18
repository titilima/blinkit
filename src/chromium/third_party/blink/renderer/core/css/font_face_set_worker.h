// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: font_face_set_worker.h
// Description: FontFaceSetWorker Class
//      Author: Ziming Li
//     Created: 2020-09-18
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_CSS_FONT_FACE_SET_WORKER_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_CSS_FONT_FACE_SET_WORKER_H_

#include "base/macros.h"
#include "third_party/blink/renderer/core/css/font_face.h"
#include "third_party/blink/renderer/core/css/font_face_set.h"
#include "third_party/blink/renderer/core/css/offscreen_font_selector.h"
#include "third_party/blink/renderer/core/dom/pausable_object.h"
// BKTODO: #include "third_party/blink/renderer/core/workers/worker_global_scope.h"
#include "third_party/blink/renderer/platform/async_method_runner.h"
#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/wtf/allocator.h"

namespace blink {

class Font;

class CORE_EXPORT FontFaceSetWorker final
    : public FontFaceSet,
      public Supplement<WorkerGlobalScope> {
  USING_GARBAGE_COLLECTED_MIXIN(FontFaceSetWorker);

 public:
  static const char kSupplementName[];

  ~FontFaceSetWorker() override;

#if 0 // BKTODO:
  ScriptPromise ready(ScriptState*) override;
#endif

  AtomicString status() const override;

  WorkerGlobalScope* GetWorker() const;

  // FontFace::LoadFontCallback
  void NotifyLoaded(FontFace*) override;
  void NotifyError(FontFace*) override;

  void BeginFontLoading(FontFace*);

  static FontFaceSetWorker* From(WorkerGlobalScope&);

 protected:
  bool InActiveContext() const override { return true; }
#if 0 // BKTODO:
  FontSelector* GetFontSelector() const override {
    return GetWorker()->GetFontSelector();
  }
#endif
  // For workers, this is always an empty list.
  const HeapLinkedHashSet<Member<FontFace>>& CSSConnectedFontFaceList()
      const override {
    DCHECK(
        GetFontSelector()->GetFontFaceCache()->CssConnectedFontFaces().size() ==
        0);
    return GetFontSelector()->GetFontFaceCache()->CssConnectedFontFaces();
  }

  bool ResolveFontStyle(const String&, Font&) override;

 private:
  static FontFaceSetWorker* Create(WorkerGlobalScope& worker) {
    ASSERT(false); // BKTODO:
    return nullptr;
#if 0
    return new FontFaceSetWorker(worker);
#endif
  }

  explicit FontFaceSetWorker(WorkerGlobalScope&);

  void FireDoneEventIfPossible() override;
  DISALLOW_COPY_AND_ASSIGN(FontFaceSetWorker);
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_CSS_FONT_FACE_SET_WORKER_H_
