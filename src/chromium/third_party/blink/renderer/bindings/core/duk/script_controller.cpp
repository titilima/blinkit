// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: script_controller.cpp
// Description: ScriptController Class
//      Author: Ziming Li
//     Created: 2020-01-17
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2008, 2009 Google Inc. All rights reserved.
 * Copyright (C) 2009 Apple Inc. All rights reserved.
 * Copyright (C) 2014 Opera Software ASA. All rights reserved.
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

#include "script_controller.h"

#include "base/memory/ptr_util.h"
#include "blinkit/crawler/crawler_impl.h"
#include "blinkit/js/context_impl.h"
#include "third_party/blink/renderer/bindings/core/duk/duk.h"
#include "third_party/blink/renderer/bindings/core/duk/script_source_code.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"

using namespace BlinKit;

namespace blink {

static void CommonCallback(ContextImpl *ctxImpl, duk_context *ctx)
{
    if (!duk_is_error(ctx, -1))
        return;

#ifndef NDEBUG
    duk_get_prop_string(ctx, -1, "stack");
#endif
    std::string str = Duk::To<std::string>(ctx, -1);
    ctxImpl->ConsoleOutput(BK_CONSOLE_ERROR, str.c_str());
}

ScriptController::ScriptController(LocalFrame &frame) : m_frame(frame)
{
}

ScriptController::~ScriptController(void) = default;

void ScriptController::ClearForClose(void)
{
    m_context.reset();
}

void ScriptController::ClearWindowProxy(void)
{
    if (m_context)
        m_context->Reset();
}

std::unique_ptr<ScriptController> ScriptController::Create(LocalFrame &frame)
{
    return base::WrapUnique(new ScriptController(frame));
}

ContextImpl& ScriptController::EnsureContext(void)
{
    if (!m_context)
    {
        m_context = std::make_unique<ContextImpl>(*m_frame);
        m_context->Reset();
    }
    return *m_context;
}

void ScriptController::ExecuteScriptInMainWorld(const ScriptSourceCode &sourceCode, const BkURL &baseURL)
{
    ContextImpl &ctx = EnsureContext();
    const ContextImpl::Callback callback = std::bind(CommonCallback, &ctx, std::placeholders::_1);
    ctx.Eval(sourceCode.Source(), callback, sourceCode.FileName().c_str());
}

bool ScriptController::ScriptEnabled(void)
{
    bool ret = true;
    if (CrawlerImpl *crawler = ToCrawlerImpl(m_frame->Client()))
    {
        std::string s;
        if (crawler->GetConfig(BK_CFG_SCRIPT_DISABLED, s) && !s.empty())
            ret = false;
    }
    return ret;
}

void ScriptController::UpdateDocument(void)
{
    if (m_context)
        m_context->Reset();
}

} // namespace blink
