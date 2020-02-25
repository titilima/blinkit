// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: base_fetch_context.h
// Description: BaseFetchContext Class
//      Author: Ziming Li
//     Created: 2019-10-10
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BLINKIT_BLINK_BASE_FETCH_CONTEXT_H
#define BLINKIT_BLINK_BASE_FETCH_CONTEXT_H

#pragma once

#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/loader/fetch/fetch_client_settings_object.h"
#include "third_party/blink/renderer/platform/loader/fetch/fetch_context.h"

namespace blink {

class BaseFetchContext : public FetchContext
{
protected:
    explicit BaseFetchContext(const std::shared_ptr<base::SingleThreadTaskRunner> &taskRunner);

    virtual const FetchClientSettingsObject* GetFetchClientSettingsObject(void) const = 0;
    // Used for security checks.
    virtual bool AllowScriptFromSource(const BlinKit::BkURL &url) const { return true; }

    // FetchContext overrides
    void AddAdditionalRequestHeaders(ResourceRequest &request, FetchResourceType type) override;
    std::optional<ResourceRequestBlockedReason> CanRequest(ResourceType type, const ResourceRequest &resourceRequest,
        const BlinKit::BkURL &url, const ResourceLoaderOptions &options, ResourceRequest::RedirectStatus redirectStatus) const override;
};

}  // namespace blink

#endif  // BLINKIT_BLINK_BASE_FETCH_CONTEXT_H
