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
#include "blinkit/js/crawler_context.h"
#include "third_party/blink/renderer/bindings/core/duk/duk.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_element.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_window.h"
#include "third_party/blink/renderer/bindings/core/duk/script_source_code.h"
#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"

using namespace BlinKit;

namespace blink {

static const char NativeContext[] = "nativeContext";

ScriptController::ScriptController(LocalFrame &frame, const PrototypeMap &prototypeMap)
    : m_frame(frame), m_prototypeMap(prototypeMap)
{
}

ScriptController::~ScriptController(void) = default;

void ScriptController::Attach(duk_context *ctx, duk_idx_t globalStashIndex)
{
    duk_push_pointer(ctx, this);
    duk_put_prop_string(ctx, globalStashIndex, NativeContext);
}

void ScriptController::ClearForClose(void)
{
    DestroyDukSession();
}

void ScriptController::ClearWindowProxy(void)
{
    if (IsDukSessionDirty())
        DestroyDukSession();
}

void ScriptController::ConsoleOutput(int type, const char *msg)
{
    ContextImpl::DefaultConsoleOutput(type, msg);
}

std::unique_ptr<ScriptController> ScriptController::Create(LocalFrame &frame)
{
    bool isCrawler = frame.Client()->IsCrawler();
#ifdef BLINKIT_CRAWLER_ONLY
    ASSERT(isCrawler);
    return std::make_unique<CrawlerContext>(frame);
#else
    if (isCrawler)
    {
        return std::make_unique<CrawlerContext>(frame);
    }
    else
    {
        ASSERT(false); // BKTODO:
        return nullptr;
    }
#endif
}

void ScriptController::ExecuteScriptInMainWorld(const ScriptSourceCode &sourceCode, const GURL &baseURL)
{
    const auto callback = [this](duk_context *ctx) {
        if (!duk_is_error(ctx, -1))
            return;

#ifndef NDEBUG
        duk_get_prop_string(ctx, -1, "stack");
#endif
        std::string str = Duk::To<std::string>(ctx, -1);
        ConsoleOutput(BK_CONSOLE_ERROR, str.c_str());
    };
    ContextImpl::Eval(sourceCode.Source(), callback, sourceCode.Url().ExtractFileName().c_str());
}

ScriptController* ScriptController::From(duk_context *ctx)
{
    Duk::StackGuard _(ctx);

    duk_push_global_stash(ctx);
    duk_get_prop_string(ctx, -1, NativeContext);
    return reinterpret_cast<ScriptController *>(duk_get_pointer(ctx, -1));
}

ScriptController* ScriptController::From(ExecutionContext *executionContext)
{
    if (executionContext->IsDocument())
    {
        Document *document = static_cast<Document *>(executionContext);
        ScriptController &ctx = document->GetFrame()->GetScriptController();
        return &ctx;
    }

    NOTREACHED();
    return nullptr;
}

const char* ScriptController::LookupPrototypeName(const std::string &tagName) const
{
    auto it = m_prototypeMap.find(tagName);
    return m_prototypeMap.end() != it ? it->second : DukElement::ProtoName;
}

void ScriptController::UpdateDocument(void)
{
#ifdef BLINKIT_CRAWLER_ONLY
    if (m_frame.Loader().StateMachine()->CreatingInitialEmptyDocument())
        return;
#else
    if (m_frame.Client()->IsCrawler())
    {
        if (m_frame.Loader().StateMachine()->CreatingInitialEmptyDocument())
            return;
    }
#endif

    duk_context *ctx = EnsureDukSession();

    Duk::StackGuard _(ctx);

    duk_push_global_stash(ctx);
    RegisterPrototypes(ctx, duk_normalize_index(ctx, -1));

    DukWindow::Attach(ctx, *(m_frame.DomWindow()));
}

} // namespace blink
