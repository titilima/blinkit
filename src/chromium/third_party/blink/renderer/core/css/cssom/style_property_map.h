// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: style_property_map.h
// Description: StylePropertyMap Class
//      Author: Ziming Li
//     Created: 2020-09-18
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_CSS_CSSOM_STYLE_PROPERTY_MAP_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_CSS_CSSOM_STYLE_PROPERTY_MAP_H_

#include "base/macros.h"
#include "third_party/blink/renderer/core/css/cssom/style_property_map_read_only.h"
#include "third_party/blink/renderer/core/execution_context/execution_context.h"

namespace blink {

class ExceptionState;
class ExecutionContext;

class CORE_EXPORT StylePropertyMap : public StylePropertyMapReadOnly {
  DEFINE_WRAPPERTYPEINFO();

 public:
#if 0 // BKTODO:
  void set(const ExecutionContext*,
           const String& property_name,
           const HeapVector<CSSStyleValueOrString>& values,
           ExceptionState&);
  void append(const ExecutionContext*,
              const String& property_name,
              const HeapVector<CSSStyleValueOrString>& values,
              ExceptionState&);
#endif
  void remove(const String& property_name, ExceptionState&);
  void clear();

 protected:
  virtual void SetProperty(CSSPropertyID, const CSSValue&) = 0;
  virtual bool SetShorthandProperty(CSSPropertyID,
                                    const String&,
                                    SecureContextMode) = 0;
  virtual void SetCustomProperty(const AtomicString&, const CSSValue&) = 0;
  virtual void RemoveProperty(CSSPropertyID) = 0;
  virtual void RemoveCustomProperty(const AtomicString&) = 0;
  virtual void RemoveAllProperties() = 0;

  StylePropertyMap() = default;

 private:
  DISALLOW_COPY_AND_ASSIGN(StylePropertyMap);
};

}  // namespace blink

#endif
