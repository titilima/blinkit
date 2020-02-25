// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: fetch_client_settings_object_impl.h
// Description: FetchClientSettingsObjectImpl Class
//      Author: Ziming Li
//     Created: 2020-02-17
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BLINKIT_BLINK_FETCH_CLIENT_SETTINGS_OBJECT_IMPL_H
#define BLINKIT_BLINK_FETCH_CLIENT_SETTINGS_OBJECT_IMPL_H

#pragma once

#include "third_party/blink/renderer/platform/heap/member.h"
#include "third_party/blink/renderer/platform/loader/fetch/fetch_client_settings_object.h"

namespace blink {

class ExecutionContext;

// This is an implementation of FetchClientSettingsObject. As opposed to
// FetchClientSettingsObjectSnapshot, this refers to up-to-date values of the
// settings object.
//
// This class should be used for resource loading other than main worker
// (worklet) scripts. For the main scripts, FetchClientSettingsObjectSnapshot
// should be used. See the class level comments on that class.
class FetchClientSettingsObjectImpl final : public FetchClientSettingsObject
{
public:
    explicit FetchClientSettingsObjectImpl(ExecutionContext &);
    ~FetchClientSettingsObjectImpl(void) override = default;

    const String GetOutgoingReferrer(void) const override;
private:
    Member<ExecutionContext> m_executionContext;
};

} // namespace blink

#endif // BLINKIT_BLINK_FETCH_CLIENT_SETTINGS_OBJECT_IMPL_H
