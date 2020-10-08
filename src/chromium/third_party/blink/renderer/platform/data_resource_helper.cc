// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: data_resource_helper.cc
// Description: GetDataResourceAsASCIIString
//      Author: Ziming Li
//     Created: 2020-10-08
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/platform/data_resource_helper.h"

#include "third_party/blink/public/platform/platform.h"
// BKTODO: #include "third_party/blink/public/platform/web_data.h"
#include "third_party/blink/renderer/platform/wtf/text/string_builder.h"

namespace blink {

String GetDataResourceAsASCIIString(const char* resource) {
  StringBuilder builder;
  ASSERT(false); // BKTODO:
#if 0
  const WebData& resource_data = Platform::Current()->GetDataResource(resource);
  builder.ReserveCapacity(resource_data.size());
  resource_data.ForEachSegment([&builder](const char* segment,
                                          size_t segment_size,
                                          size_t segment_offset) {
    builder.Append(segment, segment_size);
    return true;
  });
#endif

  String data_string = builder.ToString();
  DCHECK(!data_string.IsEmpty());
  DCHECK(data_string.ContainsOnlyASCII());
  return data_string;
}

}  // namespace blink
