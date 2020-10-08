// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: style_path.h
// Description: StylePath Class
//      Author: Ziming Li
//     Created: 2020-10-08
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_STYLE_STYLE_PATH_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_STYLE_STYLE_PATH_H_

#include <memory>
#include "base/memory/scoped_refptr.h"
#include "third_party/blink/renderer/core/style/basic_shapes.h"

namespace blink {

class CSSValue;
class Path;
class SVGPathByteStream;

class StylePath final : public BasicShape {
 public:
#if 0 // BKTODO:
  static scoped_refptr<StylePath> Create(std::unique_ptr<SVGPathByteStream>);
#endif
  ~StylePath() override;

  static StylePath* EmptyPath();

  const Path& GetPath() const;
  float length() const;
  bool IsClosed() const;

#if 0 // BKTODO:
  const SVGPathByteStream& ByteStream() const { return *byte_stream_; }
#endif

  CSSValue* ComputedCSSValue() const;

  void GetPath(Path&, const FloatRect&) override;
  bool operator==(const BasicShape&) const override;

  ShapeType GetType() const override { return kStylePathType; }

 private:
#if 0 // BKTODO:
  explicit StylePath(std::unique_ptr<SVGPathByteStream>);

  std::unique_ptr<SVGPathByteStream> byte_stream_;
#endif
  mutable std::unique_ptr<Path> path_;
  mutable float path_length_;
};

DEFINE_BASICSHAPE_TYPE_CASTS(StylePath);

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_STYLE_STYLE_PATH_H_
