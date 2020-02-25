// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: fetch_client_settings_object.h
// Description: FetchClientSettingsObject Class
//      Author: Ziming Li
//     Created: 2020-02-17
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BLINKIT_BLINK_FETCH_CLIENT_SETTINGS_OBJECT_H
#define BLINKIT_BLINK_FETCH_CLIENT_SETTINGS_OBJECT_H

#pragma once

#include "third_party/blink/renderer/platform/heap/garbage_collected.h"
#include "third_party/blink/renderer/platform/wtf/text/wtf_string.h"

namespace blink {

// This is a partial interface of the "settings object" concept defined in the
// HTML spec:
// https://html.spec.whatwg.org/multipage/webappapis.html#settings-object
//
// This is also a partial interface of the "fetch client settings object" used
// in module script fetch. Other part of the "fetch client settings object" is
// currently implemented by ResourceFetcher and FetchContext, and this class is
// used together with them.
// https://html.spec.whatwg.org/multipage/webappapis.html#fetch-a-module-worker-script-tree
class FetchClientSettingsObject : public GarbageCollectedFinalized<FetchClientSettingsObject>
{
public:
    virtual ~FetchClientSettingsObject(void) = default;

    // "referrerURL" used in the "Determine request's Referrer" algorithm:
    // https://w3c.github.io/webappsec-referrer-policy/#determine-requests-referrer
    virtual const String GetOutgoingReferrer(void) const = 0;
};

} // namespace blink

#endif // BLINKIT_BLINK_FETCH_CLIENT_SETTINGS_OBJECT_H
