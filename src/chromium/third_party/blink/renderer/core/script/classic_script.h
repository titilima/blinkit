// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: classic_script.h
// Description: ClassicScript Class
//      Author: Ziming Li
//     Created: 2020-02-02
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BLINKIT_BLINK_CLASSIC_SCRIPT_H
#define BLINKIT_BLINK_CLASSIC_SCRIPT_H

#pragma once

#include "third_party/blink/renderer/bindings/core/duk/script_source_code.h"
#include "third_party/blink/renderer/core/loader/resource/script_resource.h"
#include "third_party/blink/renderer/core/script/script.h"

namespace blink {

class ClassicScript final : public Script
{
public:
    static std::unique_ptr<ClassicScript> Create(const ScriptSourceCode &scriptSourceCode, const BlinKit::BkURL &baseURL);

    const ScriptSourceCode& GetScriptSourceCode(void) const { return m_scriptSourceCode; }
private:
    ClassicScript(const ScriptSourceCode &scriptSourceCode, const BlinKit::BkURL &baseURL);

    ScriptType GetScriptType(void) const override { return ScriptType::kClassic; }
    void RunScript(LocalFrame *frame) const override;

    const ScriptSourceCode m_scriptSourceCode;
};

} // namespace blink

#endif // BLINKIT_BLINK_CLASSIC_SCRIPT_H
