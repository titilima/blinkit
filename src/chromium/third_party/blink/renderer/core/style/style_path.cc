// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: style_path.cc
// Description: StylePath Class
//      Author: Ziming Li
//     Created: 2020-10-08
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/style/style_path.h"

#include <limits>
#include <memory>
#include <utility>

#include "third_party/blink/renderer/core/css/css_path_value.h"
#if 0 // BKTODO:
#include "third_party/blink/renderer/core/svg/svg_path_byte_stream.h"
#include "third_party/blink/renderer/core/svg/svg_path_utilities.h"
#endif
#include "third_party/blink/renderer/platform/graphics/path.h"

namespace blink {

#if 0 // BKTODO:
StylePath::StylePath(std::unique_ptr<SVGPathByteStream> path_byte_stream)
    : byte_stream_(std::move(path_byte_stream)),
      path_length_(std::numeric_limits<float>::quiet_NaN()) {
  DCHECK(byte_stream_);
}

StylePath::~StylePath() = default;

scoped_refptr<StylePath> StylePath::Create(
    std::unique_ptr<SVGPathByteStream> path_byte_stream) {
  return base::AdoptRef(new StylePath(std::move(path_byte_stream)));
}
#endif

StylePath* StylePath::EmptyPath() {
  ASSERT(false); // BKTODO:
  return nullptr;
#if 0
  DEFINE_STATIC_REF(StylePath, empty_path,
                    StylePath::Create(SVGPathByteStream::Create()));
  return empty_path;
#endif
}

const Path& StylePath::GetPath() const {
  if (!path_) {
    ASSERT(false); // BKTODO:
#if 0
    path_ = std::make_unique<Path>();
    BuildPathFromByteStream(*byte_stream_, *path_);
#endif
  }
  return *path_;
}

float StylePath::length() const {
  if (std::isnan(path_length_))
    path_length_ = GetPath().length();
  return path_length_;
}

bool StylePath::IsClosed() const {
  return GetPath().IsClosed();
}

CSSValue* StylePath::ComputedCSSValue() const {
  ASSERT(false); // BKTODO:
  return nullptr;
#if 0
  return cssvalue::CSSPathValue::Create(const_cast<StylePath*>(this),
                                        kTransformToAbsolute);
#endif
}

bool StylePath::operator==(const BasicShape& o) const {
  if (!IsSameType(o))
    return false;
  const StylePath& other = ToStylePath(o);
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return *byte_stream_ == *other.byte_stream_;
#endif
}

void StylePath::GetPath(Path&, const FloatRect&) {
  // Callers should use GetPath() overload, which avoids making a copy.
  NOTREACHED();
}

}  // namespace blink
