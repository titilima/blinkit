// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: fetch_client_settings_object_impl.cpp
// Description: FetchClientSettingsObjectImpl Class
//      Author: Ziming Li
//     Created: 2020-02-17
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "fetch_client_settings_object_impl.h"

#include "third_party/blink/renderer/core/execution_context/execution_context.h"
#include "third_party/blink/renderer/platform/wtf/wtf.h"

namespace blink {

FetchClientSettingsObjectImpl::FetchClientSettingsObjectImpl(ExecutionContext &executionContext)
    : m_executionContext(executionContext)
{
    ASSERT(IsMainThread());
}

const String FetchClientSettingsObjectImpl::GetOutgoingReferrer(void) const
{
    ASSERT(IsMainThread());
    return m_executionContext->OutgoingReferrer();
}

} // namespace blink
