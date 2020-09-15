// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: css_path_value.cc
// Description: CSSPathValue Class
//      Author: Ziming Li
//     Created: 2020-09-15
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/css/css_path_value.h"

#include <memory>
#include "third_party/blink/renderer/core/style/style_path.h"
// BKTODO: #include "third_party/blink/renderer/core/svg/svg_path_utilities.h"
#include "third_party/blink/renderer/platform/heap/persistent.h"

namespace blink {

namespace cssvalue {

#if 0 // BKTODO:
CSSPathValue* CSSPathValue::Create(
    scoped_refptr<StylePath> style_path,
    PathSerializationFormat serialization_format) {
  return new CSSPathValue(std::move(style_path), serialization_format);
}

CSSPathValue* CSSPathValue::Create(
    std::unique_ptr<SVGPathByteStream> path_byte_stream,
    PathSerializationFormat serialization_format) {
  return CSSPathValue::Create(StylePath::Create(std::move(path_byte_stream)),
                              serialization_format);
}

CSSPathValue::CSSPathValue(scoped_refptr<StylePath> style_path,
                           PathSerializationFormat serialization_format)
    : CSSValue(kPathClass),
      style_path_(std::move(style_path)),
      serialization_format_(serialization_format) {
  DCHECK(style_path_);
}
#endif

namespace {

CSSPathValue* CreatePathValue() {
  ASSERT(false); // BKTODO:
  return nullptr;
#if 0
  std::unique_ptr<SVGPathByteStream> path_byte_stream =
      SVGPathByteStream::Create();
  // Need to be registered as LSan ignored, as it will be reachable and
  // separately referred to by emptyPathValue() callers.
  LEAK_SANITIZER_IGNORE_OBJECT(path_byte_stream.get());
  return CSSPathValue::Create(std::move(path_byte_stream));
#endif
}

}  // namespace

CSSPathValue& CSSPathValue::EmptyPathValue() {
  DEFINE_STATIC_LOCAL(Persistent<CSSPathValue>, empty, (CreatePathValue()));
  return *empty;
}

String CSSPathValue::CustomCSSText() const {
  ASSERT(false); // BKTODO:
  return String();
#if 0
  return "path(\"" +
         BuildStringFromByteStream(ByteStream(), serialization_format_) + "\")";
#endif
}

bool CSSPathValue::Equals(const CSSPathValue& other) const {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return ByteStream() == other.ByteStream();
#endif
}

void CSSPathValue::TraceAfterDispatch(blink::Visitor* visitor) {
  CSSValue::TraceAfterDispatch(visitor);
}

}  // namespace cssvalue
}  // namespace blink
