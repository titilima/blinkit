// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: script_source_code.cpp
// Description: ScriptSourceCode Class
//      Author: Ziming Li
//     Created: 2020-02-05
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "script_source_code.h"

#include "third_party/blink/renderer/core/loader/resource/script_resource.h"

namespace blink {

ScriptSourceCode::ScriptSourceCode(
    const String &source,
    ScriptSourceLocationType sourceLocationType,
    const BlinKit::BkURL &url,
    const TextPosition &startPosition)
    : m_source(source.StdUtf8())
{
}

ScriptSourceCode::ScriptSourceCode(
    ScriptStreamer *streamer,
    ScriptResource *resource,
    ScriptStreamer::NotStreamingReason reason)
    : m_source(resource->SourceText())
    , m_URL(resource->GetResponse().Url().StripFragmentIdentifier())
{
    ASSERT(!streamer == (reason != ScriptStreamer::NotStreamingReason::kInvalid));
}

ScriptSourceCode::~ScriptSourceCode(void) = default;

} // namespace blink
