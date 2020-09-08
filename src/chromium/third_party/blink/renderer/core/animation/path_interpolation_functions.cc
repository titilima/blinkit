// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: path_interpolation_functions.cc
// Description: PathInterpolationFunctions Class
//      Author: Ziming Li
//     Created: 2020-09-08
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/core/animation/path_interpolation_functions.h"

#include <memory>
#include <utility>

#include "base/memory/ptr_util.h"
// BKTODO: #include "third_party/blink/renderer/core/animation/interpolated_svg_path_source.h"
#include "third_party/blink/renderer/core/animation/interpolation_environment.h"
// BKTODO: #include "third_party/blink/renderer/core/animation/svg_path_seg_interpolation_functions.h"
#include "third_party/blink/renderer/core/css/css_path_value.h"
// BKTODO: #include "third_party/blink/renderer/core/svg/svg_path.h"
// BKTODO: #include "third_party/blink/renderer/core/svg/svg_path_byte_stream_builder.h"
// BKTODO: #include "third_party/blink/renderer/core/svg/svg_path_byte_stream_source.h"
// BKTODO: #include "third_party/blink/renderer/core/svg/svg_path_parser.h"

namespace blink {

#if 0 // BKTODO:
class SVGPathNonInterpolableValue : public NonInterpolableValue {
 public:
  ~SVGPathNonInterpolableValue() override = default;

  static scoped_refptr<SVGPathNonInterpolableValue> Create(
      Vector<SVGPathSegType>& path_seg_types) {
    return base::AdoptRef(new SVGPathNonInterpolableValue(path_seg_types));
  }

  const Vector<SVGPathSegType>& PathSegTypes() const { return path_seg_types_; }

  DECLARE_NON_INTERPOLABLE_VALUE_TYPE();

 private:
  SVGPathNonInterpolableValue(Vector<SVGPathSegType>& path_seg_types) {
    path_seg_types_.swap(path_seg_types);
  }

  Vector<SVGPathSegType> path_seg_types_;
};

DEFINE_NON_INTERPOLABLE_VALUE_TYPE(SVGPathNonInterpolableValue);
DEFINE_NON_INTERPOLABLE_VALUE_TYPE_CASTS(SVGPathNonInterpolableValue);
#endif

enum PathComponentIndex : unsigned {
  kPathArgsIndex,
  kPathNeutralIndex,
  kPathComponentIndexCount,
};

#if 0 // BKTODO:
InterpolationValue PathInterpolationFunctions::ConvertValue(
    const SVGPathByteStream& byte_stream,
    CoordinateConversion coordinateConversion) {
  SVGPathByteStreamSource path_source(byte_stream);
  wtf_size_t length = 0;
  PathCoordinates current_coordinates;
  Vector<std::unique_ptr<InterpolableValue>> interpolable_path_segs;
  Vector<SVGPathSegType> path_seg_types;

  while (path_source.HasMoreData()) {
    const PathSegmentData segment = path_source.ParseSegment();
    interpolable_path_segs.push_back(
        SVGPathSegInterpolationFunctions::ConsumePathSeg(segment,
                                                         current_coordinates));
    SVGPathSegType seg_type = segment.command;
    if (coordinateConversion == ForceAbsolute)
      seg_type = ToAbsolutePathSegType(seg_type);
    path_seg_types.push_back(seg_type);
    length++;
  }

  std::unique_ptr<InterpolableList> path_args =
      InterpolableList::Create(length);
  for (wtf_size_t i = 0; i < interpolable_path_segs.size(); i++)
    path_args->Set(i, std::move(interpolable_path_segs[i]));

  std::unique_ptr<InterpolableList> result =
      InterpolableList::Create(kPathComponentIndexCount);
  result->Set(kPathArgsIndex, std::move(path_args));
  result->Set(kPathNeutralIndex, InterpolableNumber::Create(0));

  return InterpolationValue(
      std::move(result), SVGPathNonInterpolableValue::Create(path_seg_types));
}
#endif

InterpolationValue PathInterpolationFunctions::ConvertValue(
    const StylePath* style_path,
    CoordinateConversion coordinateConversion) {
  ASSERT(false); // BKTODO:
  return nullptr;
#if 0
  if (style_path)
    return ConvertValue(style_path->ByteStream(), coordinateConversion);

  std::unique_ptr<SVGPathByteStream> empty_path = SVGPathByteStream::Create();
  return ConvertValue(*empty_path, ForceAbsolute);
#endif
}

#if 0 // BKTODO:
class UnderlyingPathSegTypesChecker
    : public InterpolationType::ConversionChecker {
 public:
  ~UnderlyingPathSegTypesChecker() final = default;

  static std::unique_ptr<UnderlyingPathSegTypesChecker> Create(
      const InterpolationValue& underlying) {
    return base::WrapUnique(
        new UnderlyingPathSegTypesChecker(GetPathSegTypes(underlying)));
  }

 private:
  UnderlyingPathSegTypesChecker(const Vector<SVGPathSegType>& path_seg_types)
      : path_seg_types_(path_seg_types) {}

  static const Vector<SVGPathSegType>& GetPathSegTypes(
      const InterpolationValue& underlying) {
    return ToSVGPathNonInterpolableValue(*underlying.non_interpolable_value)
        .PathSegTypes();
  }

  bool IsValid(const InterpolationEnvironment&,
               const InterpolationValue& underlying) const final {
    return path_seg_types_ == GetPathSegTypes(underlying);
  }

  Vector<SVGPathSegType> path_seg_types_;
};
#endif

InterpolationValue PathInterpolationFunctions::MaybeConvertNeutral(
    const InterpolationValue& underlying,
    InterpolationType::ConversionCheckers& conversion_checkers) {
  ASSERT(false); // BKTODO:
  return nullptr;
#if 0
  conversion_checkers.push_back(
      UnderlyingPathSegTypesChecker::Create(underlying));
  std::unique_ptr<InterpolableList> result =
      InterpolableList::Create(kPathComponentIndexCount);
  result->Set(kPathArgsIndex, ToInterpolableList(*underlying.interpolable_value)
                                  .Get(kPathArgsIndex)
                                  ->CloneAndZero());
  result->Set(kPathNeutralIndex, InterpolableNumber::Create(1));
  return InterpolationValue(std::move(result),
                            underlying.non_interpolable_value.get());
#endif
}

#if 0 // BKTODO:
static bool PathSegTypesMatch(const Vector<SVGPathSegType>& a,
                              const Vector<SVGPathSegType>& b) {
  if (a.size() != b.size())
    return false;

  for (wtf_size_t i = 0; i < a.size(); i++) {
    if (ToAbsolutePathSegType(a[i]) != ToAbsolutePathSegType(b[i]))
      return false;
  }

  return true;
}
#endif

PairwiseInterpolationValue PathInterpolationFunctions::MaybeMergeSingles(
    InterpolationValue&& start,
    InterpolationValue&& end) {
  ASSERT(false); // BKTODO:
  return nullptr;
#if 0
  const Vector<SVGPathSegType>& start_types =
      ToSVGPathNonInterpolableValue(*start.non_interpolable_value)
          .PathSegTypes();
  const Vector<SVGPathSegType>& end_types =
      ToSVGPathNonInterpolableValue(*end.non_interpolable_value).PathSegTypes();
  if (start_types.size() == 0 || !PathSegTypesMatch(start_types, end_types))
    return nullptr;

  return PairwiseInterpolationValue(std::move(start.interpolable_value),
                                    std::move(end.interpolable_value),
                                    std::move(end.non_interpolable_value));
#endif
}

void PathInterpolationFunctions::Composite(
    UnderlyingValueOwner& underlying_value_owner,
    double underlying_fraction,
    const InterpolationType& type,
    const InterpolationValue& value) {
  const InterpolableList& list = ToInterpolableList(*value.interpolable_value);
  double neutral_component =
      ToInterpolableNumber(list.Get(kPathNeutralIndex))->Value();

  if (neutral_component == 0) {
    underlying_value_owner.Set(type, value);
    return;
  }

  ASSERT(false); // BKTODO:
#if 0
  DCHECK(PathSegTypesMatch(
      ToSVGPathNonInterpolableValue(
          *underlying_value_owner.Value().non_interpolable_value)
          .PathSegTypes(),
      ToSVGPathNonInterpolableValue(*value.non_interpolable_value)
          .PathSegTypes()));
  underlying_value_owner.MutableValue().interpolable_value->ScaleAndAdd(
      neutral_component, *value.interpolable_value);
  underlying_value_owner.MutableValue().non_interpolable_value =
      value.non_interpolable_value.get();
#endif
}

#if 0 // BKTODO:
std::unique_ptr<SVGPathByteStream> PathInterpolationFunctions::AppliedValue(
    const InterpolableValue& interpolable_value,
    const NonInterpolableValue* non_interpolable_value) {
  std::unique_ptr<SVGPathByteStream> path_byte_stream =
      SVGPathByteStream::Create();
  InterpolatedSVGPathSource source(
      ToInterpolableList(
          *ToInterpolableList(interpolable_value).Get(kPathArgsIndex)),
      ToSVGPathNonInterpolableValue(non_interpolable_value)->PathSegTypes());
  SVGPathByteStreamBuilder builder(*path_byte_stream);
  SVGPathParser::ParsePath(source, builder);
  return path_byte_stream;
}
#endif

}  // namespace blink
