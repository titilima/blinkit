// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: base_fetch_context.cpp
// Description: BaseFetchContext Class
//      Author: Ziming Li
//     Created: 2019-10-10
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base_fetch_context.h"

#include "third_party/blink/renderer/platform/loader/fetch/resource.h"

using namespace BlinKit;

namespace blink {

BaseFetchContext::BaseFetchContext(const std::shared_ptr<base::SingleThreadTaskRunner> &taskRunner)
    : FetchContext(taskRunner)
{
}

void BaseFetchContext::AddAdditionalRequestHeaders(ResourceRequest &request, FetchResourceType type)
{
    if (kFetchMainResource == type)
        return;

    ASSERT(false); // BKTODO:
#if 0
            // TODO(domfarolino): we can probably *just set* the HTTP `Referer` here
        // no matter what now.
    if (!request.DidSetHTTPReferrer()) {
        String referrer_to_use = request.ReferrerString();
        ReferrerPolicy referrer_policy_to_use = request.GetReferrerPolicy();

        if (referrer_to_use == Referrer::ClientReferrerString())
            referrer_to_use = GetFetchClientSettingsObject()->GetOutgoingReferrer();

        if (referrer_policy_to_use == kReferrerPolicyDefault) {
            referrer_policy_to_use =
                GetFetchClientSettingsObject()->GetReferrerPolicy();
        }

        // TODO(domfarolino): Stop storing ResourceRequest's referrer as a header
        // and store it elsewhere. See https://crbug.com/850813.
        request.SetHTTPReferrer(SecurityPolicy::GenerateReferrer(
            referrer_policy_to_use, request.Url(), referrer_to_use));
        request.SetHTTPOriginIfNeeded(GetSecurityOrigin());
    }
    else {
        DCHECK_EQ(SecurityPolicy::GenerateReferrer(request.GetReferrerPolicy(),
            request.Url(),
            request.HttpReferrer())
            .referrer,
            request.HttpReferrer());
        request.SetHTTPOriginToMatchReferrerIfNeeded();
    }
#endif
}

std::optional<ResourceRequestBlockedReason> BaseFetchContext::CanRequest(
    ResourceType type,
    const ResourceRequest &resourceRequest,
    const BkURL &url,
    const ResourceLoaderOptions &options,
    ResourceRequest::RedirectStatus redirectStatus) const
{
    if (IsDetached())
    {
        ASSERT(false); // BKTODO:
#if 0
        if (!resource_request.GetKeepalive() ||
            redirect_status == ResourceRequest::RedirectStatus::kNoRedirect) {
            return ResourceRequestBlockedReason::kOther;
        }
#endif
    }

    if (ResourceType::kScript == type || ResourceType::kImportResource == type)
    {
        ASSERT(false); // BKTODO:
#if 0
        if (!AllowScriptFromSource(url)) {
            // TODO(estark): Use a different ResourceRequestBlockedReason here, since
            // this check has nothing to do with CSP. https://crbug.com/600795
            return ResourceRequestBlockedReason::kCSP;
        }
#endif
    }

    return std::nullopt;
}

}  // namespace blink
