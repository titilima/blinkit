// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: script.h
// Description: Script Class
//      Author: Ziming Li
//     Created: 2019-10-20
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_SCRIPT_SCRIPT_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_SCRIPT_SCRIPT_H_

#include "third_party/blink/renderer/core/core_export.h"
#include "third_party/blink/renderer/platform/bindings/script_wrappable.h"
#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/wtf/text/wtf_string.h"
#include "url/gurl.h"

namespace blink {

class LocalFrame;

// TODO(asamidoi, nhiroki): Remove this enum in favor of
// blink::mojom::ScriptType.
enum class ScriptType { kClassic, kModule };

// https://html.spec.whatwg.org/multipage/webappapis.html#concept-script
class CORE_EXPORT Script : public GarbageCollectedFinalized<Script> {
 public:
  virtual ~Script(void) = default;

  virtual ScriptType GetScriptType() const = 0;

  // https://html.spec.whatwg.org/multipage/webappapis.html#run-a-classic-script
  // or
  // https://html.spec.whatwg.org/multipage/webappapis.html#run-a-module-script,
  // depending on the script type.
  virtual void RunScript(LocalFrame*) const = 0;

  const GURL& BaseURL() const { return base_url_; }

 protected:
  explicit Script(const GURL& base_url)
      : base_url_(base_url) {}

 private:
  // https://html.spec.whatwg.org/multipage/webappapis.html#concept-script-base-url
  const GURL base_url_;
};

}  // namespace blink

#endif
