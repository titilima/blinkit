#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: script_controller_impl.h
// Description: ScriptController Class
//      Author: Ziming Li
//     Created: 2022-03-02
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
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

#ifndef BLINKIT_SCRIPT_CONTROLLER_IMPL_H
#define BLINKIT_SCRIPT_CONTROLLER_IMPL_H

#include "blinkit/blink/renderer/platform/heap/Handle.h"
#include "blinkit/blink/renderer/wtf/Noncopyable.h"

namespace blink {

class ScriptController
{
    WTF_MAKE_NONCOPYABLE(ScriptController);
public:
    static std::unique_ptr<ScriptController> Create(LocalFrame &frame);
    ~ScriptController(void);

    /**
     * Common Exports
     */
    virtual bool canExecuteScripts(ReasonForCallingCanExecuteScripts) { return true; }
    void clearForClose(void);
    void clearWindowProxy(void);
    void executeScriptInMainWorld(const ScriptSourceCode &sourceCode);
    constexpr bool shouldBypassMainWorldCSP(void) const { return true; }
    void updateDocument(void);
private:
    ScriptController(LocalFrame &frame);

    LocalFrame &m_frame;
};

} // namespace blink

#endif // BLINKIT_SCRIPT_CONTROLLER_IMPL_H
