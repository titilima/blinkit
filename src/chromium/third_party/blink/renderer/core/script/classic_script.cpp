// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: classic_script.cpp
// Description: ClassicScript Class
//      Author: Ziming Li
//     Created: 2020-02-02
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "classic_script.h"

#include "base/memory/ptr_util.h"
#include "third_party/blink/renderer/bindings/core/duk/script_controller.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"

using namespace BlinKit;

namespace blink {

ClassicScript::ClassicScript(const ScriptSourceCode &scriptSourceCode, const BkURL &baseURL)
    : Script(baseURL), m_scriptSourceCode(scriptSourceCode)
{
}

std::unique_ptr<ClassicScript> ClassicScript::Create(const ScriptSourceCode &scriptSourceCode, const BkURL &baseURL)
{
    return base::WrapUnique(new ClassicScript(scriptSourceCode, baseURL));
}

void ClassicScript::RunScript(LocalFrame *frame) const
{
    frame->GetScriptController().ExecuteScriptInMainWorld(GetScriptSourceCode(), BaseURL());
}

} // namespace blink
