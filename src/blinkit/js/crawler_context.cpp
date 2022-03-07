// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: crawler_context.cpp
// Description: CrawlerContext Class
//      Author: Ziming Li
//     Created: 2022-03-05
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#include "./crawler_context.h"

#include "blinkit/blink/renderer/bindings/core/qjs/qjs_bindings.h"

namespace BlinKit {

CrawlerContext::CrawlerContext(LocalFrame &frame) : ScriptController(frame)
{
}

void CrawlerContext::OnContextCreated(JSContext *ctx, JSValue global)
{
}

} // namespace BlinKit
