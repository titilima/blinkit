// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: script_controller.h
// Description: ScriptController Class
//      Author: Ziming Li
//     Created: 2020-01-17
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2008, 2009 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef BLINKIT_BLINK_SCRIPT_CONTROLLER_H
#define BLINKIT_BLINK_SCRIPT_CONTROLLER_H

#pragma once

#include <string>
#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/wtf/noncopyable.h"

class GURL;

namespace BlinKit {
class BrowserContext;
}

namespace blink {

class LocalFrame;
class ScriptSourceCode;

// This class exposes methods to run script in a frame (in the main world and
// in isolated worlds). An instance can be obtained by using
// LocalFrame::GetScriptController().
class ScriptController final : public GarbageCollected<ScriptController>
{
    WTF_MAKE_NONCOPYABLE(ScriptController);
public:
    static std::unique_ptr<ScriptController> Create(LocalFrame &frame);
    ~ScriptController(void);

    BlinKit::BrowserContext* GetContext(void) { return m_context.get(); }
    BlinKit::BrowserContext& EnsureContext(void);
    bool ScriptEnabled(const std::string &URL);

    void ExecuteScriptInMainWorld(const ScriptSourceCode &sourceCode, const GURL &baseURL);

    void ClearWindowProxy(void);
    void UpdateDocument(void);

    void ClearForClose(void);
private:
    ScriptController(LocalFrame &frame);

    const Member<LocalFrame> m_frame;
    std::unique_ptr<BlinKit::BrowserContext> m_context;
};

} // namespace blink

#endif // BLINKIT_BLINK_SCRIPT_CONTROLLER_H
