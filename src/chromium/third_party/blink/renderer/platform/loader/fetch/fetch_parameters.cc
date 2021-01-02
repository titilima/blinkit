// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: fetch_parameters.cc
// Description: FetchParameters Class
//      Author: Ziming Li
//     Created: 2021-01-02
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2012 Google, Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY GOOGLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "third_party/blink/renderer/platform/loader/fetch/fetch_parameters.h"

#include <memory>

#include "third_party/blink/renderer/platform/loader/fetch/resource_fetcher.h"

namespace blink {

FetchParameters::FetchParameters(const ResourceRequest& resource_request)
    : resource_request_(resource_request),
      decoder_options_(TextResourceDecoderOptions::kPlainTextContent),
      speculative_preload_type_(SpeculativePreloadType::kNotSpeculative),
      defer_(kNoDefer) {}

FetchParameters::FetchParameters(const ResourceRequest& resource_request,
                                 const ResourceLoaderOptions& options)
    : resource_request_(resource_request),
      decoder_options_(TextResourceDecoderOptions::kPlainTextContent),
      options_(options),
      speculative_preload_type_(SpeculativePreloadType::kNotSpeculative),
      defer_(kNoDefer) {}

FetchParameters::~FetchParameters() = default;

void FetchParameters::SetSpeculativePreloadType(
    SpeculativePreloadType speculative_preload_type,
    double discovery_time) {
  speculative_preload_type_ = speculative_preload_type;
}

}  // namespace blink
